## Specify phone tech before including full_phone
#$(call inherit-product, vendor/cm/config/gsm.mk)

# Release name
PRODUCT_RELEASE_NAME := Lenovo Vibe X2-EU

# Inherit some common CM stuff.
$(call inherit-product, vendor/cm/config/common_full_phone.mk)

# Inherit device configuration
$(call inherit-product, device/lenovo/x2eu/device_x2eu.mk)

## Device identifier. This must come after all inclusions
PRODUCT_DEVICE := x2eu
PRODUCT_NAME := cm_x2eu
PRODUCT_BRAND := Lenovo
PRODUCT_MODEL := Lenovo Vibe X2
PRODUCT_MANUFACTURER := LENOVO
CM_BUILD := For_EU
