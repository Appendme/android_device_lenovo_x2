/*
 * Copyright (C) 2013 Andreas Steffen
 * HSR Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "imv_swid_state.h"
#include "imv/imv_lang_string.h"
#include "imv/imv_reason_string.h"
#include "imv/imv_remediation_string.h"

#include <tncif_policy.h>

#include <utils/lexparser.h>
#include <utils/debug.h>

typedef struct private_imv_swid_state_t private_imv_swid_state_t;

/**
 * Private data of an imv_swid_state_t object.
 */
struct private_imv_swid_state_t {

	/**
	 * Public members of imv_swid_state_t
	 */
	imv_swid_state_t public;

	/**
	 * TNCCS connection ID
	 */
	TNC_ConnectionID connection_id;

	/**
	 * TNCCS connection state
	 */
	TNC_ConnectionState state;

	/**
	 * Does the TNCCS connection support long message types?
	 */
	bool has_long;

	/**
	 * Does the TNCCS connection support exclusive delivery?
	 */
	bool has_excl;

	/**
	 * Maximum PA-TNC message size for this TNCCS connection
	 */
	u_int32_t max_msg_len;

	/**
	 * Flags set for completed actions
	 */
	u_int32_t action_flags;

	/**
	 * Access Requestor ID Type
	 */
	u_int32_t ar_id_type;

	/**
	 * Access Requestor ID Value
	 */
	chunk_t ar_id_value;

	/**
	 * IMV database session associatied with TNCCS connection
	 */
	imv_session_t *session;

	/**
	 * IMV action recommendation
	 */
	TNC_IMV_Action_Recommendation rec;

	/**
	 * IMV evaluation result
	 */
	TNC_IMV_Evaluation_Result eval;

	/**
	 * IMV Scanner handshake state
	 */
	imv_swid_handshake_state_t handshake_state;

	/**
	 * TNC Reason String
	 */
	imv_reason_string_t *reason_string;

	/**
	 * IETF Remediation Instructions String
	 */
	imv_remediation_string_t *remediation_string;

};

METHOD(imv_state_t, get_connection_id, TNC_ConnectionID,
	private_imv_swid_state_t *this)
{
	return this->connection_id;
}

METHOD(imv_state_t, has_long, bool,
	private_imv_swid_state_t *this)
{
	return this->has_long;
}

METHOD(imv_state_t, has_excl, bool,
	private_imv_swid_state_t *this)
{
	return this->has_excl;
}

METHOD(imv_state_t, set_flags, void,
	private_imv_swid_state_t *this, bool has_long, bool has_excl)
{
	this->has_long = has_long;
	this->has_excl = has_excl;
}

METHOD(imv_state_t, set_max_msg_len, void,
	private_imv_swid_state_t *this, u_int32_t max_msg_len)
{
	this->max_msg_len = max_msg_len;
}

METHOD(imv_state_t, get_max_msg_len, u_int32_t,
	private_imv_swid_state_t *this)
{
	return this->max_msg_len;
}

METHOD(imv_state_t, set_action_flags, void,
	private_imv_swid_state_t *this, u_int32_t flags)
{
	this->action_flags |= flags;
}

METHOD(imv_state_t, get_action_flags, u_int32_t,
	private_imv_swid_state_t *this)
{
	return this->action_flags;
}

METHOD(imv_state_t, set_ar_id, void,
	private_imv_swid_state_t *this, u_int32_t id_type, chunk_t id_value)
{
	this->ar_id_type = id_type;
	this->ar_id_value = chunk_clone(id_value);
}

METHOD(imv_state_t, get_ar_id, chunk_t,
	private_imv_swid_state_t *this, u_int32_t *id_type)
{
	if (id_type)
	{
		*id_type = this->ar_id_type;
	}
	return this->ar_id_value;
}

METHOD(imv_state_t, set_session, void,
	private_imv_swid_state_t *this, imv_session_t *session)
{
	this->session = session;
}

METHOD(imv_state_t, get_session, imv_session_t*,
	private_imv_swid_state_t *this)
{
	return this->session;
}

METHOD(imv_state_t, change_state, void,
	private_imv_swid_state_t *this, TNC_ConnectionState new_state)
{
	this->state = new_state;
}

METHOD(imv_state_t, get_recommendation, void,
	private_imv_swid_state_t *this, TNC_IMV_Action_Recommendation *rec,
									   TNC_IMV_Evaluation_Result *eval)
{
	*rec = this->rec;
	*eval = this->eval;
}

METHOD(imv_state_t, set_recommendation, void,
	private_imv_swid_state_t *this, TNC_IMV_Action_Recommendation rec,
									   TNC_IMV_Evaluation_Result eval)
{
	this->rec = rec;
	this->eval = eval;
}

METHOD(imv_state_t, update_recommendation, void,
	private_imv_swid_state_t *this, TNC_IMV_Action_Recommendation rec,
									   TNC_IMV_Evaluation_Result eval)
{
	this->rec  = tncif_policy_update_recommendation(this->rec, rec);
	this->eval = tncif_policy_update_evaluation(this->eval, eval);
}

METHOD(imv_state_t, get_reason_string, bool,
	private_imv_swid_state_t *this, enumerator_t *language_enumerator,
	chunk_t *reason_string, char **reason_language)
{
	return FALSE;
}

METHOD(imv_state_t, get_remediation_instructions, bool,
	private_imv_swid_state_t *this, enumerator_t *language_enumerator,
	chunk_t *string, char **lang_code, char **uri)
{
	return FALSE;
}

METHOD(imv_state_t, destroy, void,
	private_imv_swid_state_t *this)
{
	DESTROY_IF(this->session);
	DESTROY_IF(this->reason_string);
	DESTROY_IF(this->remediation_string);
	free(this->ar_id_value.ptr);
	free(this);
}

METHOD(imv_swid_state_t, set_handshake_state, void,
	private_imv_swid_state_t *this, imv_swid_handshake_state_t new_state)
{
	this->handshake_state = new_state;
}

METHOD(imv_swid_state_t, get_handshake_state, imv_swid_handshake_state_t,
	private_imv_swid_state_t *this)
{
	return this->handshake_state;
}

/**
 * Described in header.
 */
imv_state_t *imv_swid_state_create(TNC_ConnectionID connection_id)
{
	private_imv_swid_state_t *this;

	INIT(this,
		.public = {
			.interface = {
				.get_connection_id = _get_connection_id,
				.has_long = _has_long,
				.has_excl = _has_excl,
				.set_flags = _set_flags,
				.set_max_msg_len = _set_max_msg_len,
				.get_max_msg_len = _get_max_msg_len,
				.set_action_flags = _set_action_flags,
				.get_action_flags = _get_action_flags,
				.set_ar_id = _set_ar_id,
				.get_ar_id = _get_ar_id,
				.set_session = _set_session,
				.get_session= _get_session,
				.change_state = _change_state,
				.get_recommendation = _get_recommendation,
				.set_recommendation = _set_recommendation,
				.update_recommendation = _update_recommendation,
				.get_reason_string = _get_reason_string,
				.get_remediation_instructions = _get_remediation_instructions,
				.destroy = _destroy,
			},
			.set_handshake_state = _set_handshake_state,
			.get_handshake_state = _get_handshake_state,
		},
		.state = TNC_CONNECTION_STATE_CREATE,
		.rec = TNC_IMV_ACTION_RECOMMENDATION_NO_RECOMMENDATION,
		.eval = TNC_IMV_EVALUATION_RESULT_DONT_KNOW,
		.connection_id = connection_id,
	);

	return &this->public.interface;
}


