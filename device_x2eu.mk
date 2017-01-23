$(call inherit-product, $(SRC_TARGET_DIR)/product/languages_full.mk)
$(call inherit-product, $(SRC_TARGET_DIR)/product/full_base_telephony.mk)

# The gps config appropriate for this device
$(call inherit-product, device/common/gps/gps_us_supl.mk)

$(call inherit-product-if-exists, vendor/lenovo/x2eu/x2eu-vendor.mk)

DEVICE_PACKAGE_OVERLAYS += device/lenovo/x2eu/overlay

LOCAL_PATH := device/lenovo/x2eu
ifeq ($(TARGET_PREBUILT_KERNEL),)
	LOCAL_KERNEL := $(LOCAL_PATH)/prebuilt/kernel
else
	LOCAL_KERNEL := $(TARGET_PREBUILT_KERNEL)
endif

TARGET_SCREEN_HEIGHT := 1920
TARGET_SCREEN_WIDTH := 1080

PRODUCT_PACKAGES += \
    libxlog

PRODUCT_LOCALES := ru_RU

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.sys.timezone=Asia/Krasnoyarsk

# various MTK ABI fixes
# this is the approach of the cm-13.0 sprout port
PRODUCT_PACKAGES += \
    libmtkabi

# Display
PRODUCT_PACKAGES += \
    libgralloc_extra \
    libion_mtk \
    libui_ext \
    libgui_ext \
    guiext-server \
    librrc \
    libmtk_drvb \

# Power
PRODUCT_PACKAGES += \
    power.mt6595 \

# Lights
PRODUCT_PACKAGES += \
    lights.mt6595 \

# Camera
PRODUCT_PACKAGES += \
    Snap \
    libnvramagentclient \

# Use CM Gello browser
#PRODUCT_PACKAGES += \
#    Gello \

# 
#PRODUCT_PACKAGES += \
#	OpenWeatherMapProvider \
#	YahooWeatherProvider \

# Audio
PRODUCT_PACKAGES += \
    audio.a2dp.default \
    audio.r_submix.default \
    audio_policy.default \
    libtinycompress \
    libtinyxml \
    libblisrc \
    libblisrc32 \

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/media_codecs.xml:system/etc/media_codecs.xml \
    $(LOCAL_PATH)/configs/media_profiles.xml:system/etc/media_profiles.xml \
    $(LOCAL_PATH)/configs/audio_policy.conf:system/etc/audio_policy.conf

# Wifi
PRODUCT_PACKAGES += \
    libwpa_client \
    hostapd \
    dhcpcd.conf \
    wpa_supplicant \
    wpa_supplicant.conf

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/hostapd/hostapd_default.conf:system/etc/hostapd/hostapd_default.conf \
    $(LOCAL_PATH)/configs/hostapd/hostapd.accept:system/etc/hostapd/hostapd.accept \
    $(LOCAL_PATH)/configs/hostapd/hostapd.deny:system/etc/hostapd/hostapd.deny

# Bluetooth
PRODUCT_PACKAGES += \
    libbt-vendor \

PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/bt_did.conf:system/etc/bluetooth/bt_did.conf

# RIL
PRODUCT_PACKAGES += \
    gsm0710muxd \
    gsm0710muxdmd2 \
    mtkrild \
    mtkrildmd2 \
    mtk-ril \
    mtk-rilmd2 \

# Telecom
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/apns-conf.xml:system/etc/apns-conf.xml \
    $(LOCAL_PATH)/configs/ecc_list.xml:system/etc/ecc_list.xml \
    $(LOCAL_PATH)/configs/spn-conf.xml:system/etc/spn-conf.xml \

# GPS
PRODUCT_COPY_FILES += \
     $(LOCAL_PATH)/configs/agps_profiles_conf2.xml:system/etc/agps_profiles_conf2.xml

PRODUCT_PACKAGES += \
    gps.mt6595 \
    mtk_agpsd \

# Vibration
#PRODUCT_PACKAGES += \
    openimmvibed \


# Keylayout
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/configs/mtk-kpd.kl:system/usr/keylayout/mtk-kpd.kl \
    $(LOCAL_PATH)/configs/mtk-tpd.kl:system/usr/keylayout/mtk-tpd.kl

# Thermal
PRODUCT_COPY_FILES += \
     $(LOCAL_PATH)/configs/.ht120.mtc:system/etc/.tp/.ht120.mtc \
     $(LOCAL_PATH)/configs/thermal.conf:system/etc/.tp/thermal.conf \
     $(LOCAL_PATH)/configs/thermal.off.conf:system/etc/.tp/thermal.off.conf \
     $(LOCAL_PATH)/configs/thermal.high.conf:system/etc/.tp/thermal.high.conf \
     $(LOCAL_PATH)/configs/thermal.low.conf:system/etc/.tp/thermal.low.conf \
     $(LOCAL_PATH)/configs/thermal.mid.conf:system/etc/.tp/thermal.mid.conf

# Ramdisk
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/root/enableswap.sh:root/enableswap.sh \
    $(LOCAL_PATH)/root/fstab.mt6595:root/fstab.mt6595 \
    $(LOCAL_PATH)/root/twrp.fstab:recovery/root/etc/twrp.fstab \
    $(LOCAL_PATH)/root/init.mt6595.rc:root/init.mt6595.rc \
    $(LOCAL_PATH)/root/init.mt6595.usb.rc:root/init.mt6595.usb.rc \
    $(LOCAL_PATH)/root/init.recovery.mt6595.rc:root/init.recovery.mt6595.rc \
    $(LOCAL_PATH)/root/init.aee.rc:root/init.aee.rc \
    $(LOCAL_PATH)/root/init.modem.rc:root/init.modem.rc \
    $(LOCAL_PATH)/root/init.project.rc:root/init.project.rc \
    $(LOCAL_PATH)/root/init.ssd.rc:root/init.ssd.rc \
    $(LOCAL_PATH)/root/init.xlog.rc:root/init.xlog.rc \
    $(LOCAL_PATH)/root/init.epdg.rc:root/init.epdg.rc \
    $(LOCAL_PATH)/root/init.mal.rc:root/init.mal.rc \
    $(LOCAL_PATH)/root/init.wfca.rc:root/init.wfca.rc \
    $(LOCAL_PATH)/root/ueventd.rc:root/ueventd.rc \
    $(LOCAL_PATH)/root/sbin/busybox:root/sbin/busybox \
    $(LOCAL_KERNEL):kernel

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/handheld_core_hardware.xml:system/etc/permissions/handheld_core_hardware.xml \
    frameworks/native/data/etc/android.hardware.bluetooth.xml:system/etc/permissions/android.hardware.bluetooth.xml \
    frameworks/native/data/etc/android.hardware.bluetooth_le.xml:system/etc/permissions/android.hardware.bluetooth_le.xml \
    frameworks/native/data/etc/android.hardware.wifi.xml:system/etc/permissions/android.hardware.wifi.xml \
    frameworks/native/data/etc/android.hardware.wifi.direct.xml:system/etc/permissions/android.hardware.wifi.direct.xml \
    frameworks/native/data/etc/android.hardware.touchscreen.multitouch.jazzhand.xml:system/etc/permissions/android.hardware.touchscreen.multitouch.jazzhand.xml \
    frameworks/native/data/etc/android.hardware.telephony.gsm.xml:system/etc/permissions/android.hardware.telephony.gsm.xml \
    frameworks/native/data/etc/android.hardware.sensor.light.xml:system/etc/permissions/android.hardware.sensor.light.xml \
    frameworks/native/data/etc/android.hardware.sensor.proximity.xml:system/etc/permissions/android.hardware.sensor.proximity.xml \
    frameworks/native/data/etc/android.hardware.sensor.stepcounter.xml:system/etc/permissions/android.hardware.sensor.stepcounter.xml \
    frameworks/native/data/etc/android.hardware.location.gps.xml:system/etc/permissions/android.hardware.location.gps.xml \
    frameworks/native/data/etc/android.hardware.camera.autofocus.xml:system/etc/permissions/android.hardware.camera.autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.flash-autofocus.xml:system/etc/permissions/android.hardware.camera.flash-autofocus.xml \
    frameworks/native/data/etc/android.hardware.camera.manual_sensor.xml:system/etc/permissions/android.hardware.camera.manual_sensor.xml \
    frameworks/native/data/etc/android.hardware.camera.manual_postprocessing.xml:system/etc/permissions/android.hardware.camera.manual_postprocessing.xml \
    frameworks/native/data/etc/android.hardware.camera.raw.xml:system/etc/permissions/android.hardware.camera.raw.xml \
    frameworks/native/data/etc/android.hardware.camera.front.xml:system/etc/permissions/android.hardware.camera.front.xml \
    frameworks/native/data/etc/android.hardware.camera.xml:system/etc/permissions/android.hardware.camera.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml \
    frameworks/native/data/etc/android.hardware.usb.accessory.xml:system/etc/permissions/android.hardware.usb.accessory.xml \
    frameworks/native/data/etc/android.software.midi.xml:system/etc/permissions/android.software.midi.xml \
    frameworks/native/data/etc/android.software.sip.voip.xml:system/etc/permissions/android.software.sip.voip.xml \

# USB
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.sys.usb.config=mtp \
    ro.hardware=mt6595 \

PRODUCT_PACKAGES += \
    Torch

PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
ro.adb.secure=0 \
ro.secure=0 \
ro.allow.mock.location=0 \
ro.debuggable=1 \
dalvik.vm.dex2oat-Xms=64m \
dalvik.vm.dex2oat-Xmx=512m \
dalvik.vm.image-dex2oat-Xms=64m \
dalvik.vm.image-dex2oat-Xmx=64m \

# Screen density
PRODUCT_AAPT_CONFIG := normal
PRODUCT_AAPT_PREF_CONFIG := xxhdpi


$(call inherit-product, build/target/product/full.mk)

$(call inherit-product, frameworks/native/build/phone-xxhdpi-2048-dalvik-heap.mk)


# vim:set ai et ts=4 sw=4 sts=4 fenc=utf-8:
