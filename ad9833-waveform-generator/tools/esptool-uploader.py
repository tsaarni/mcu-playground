
from SCons.Script import DefaultEnvironment
import os

env = DefaultEnvironment()
env.Replace(
    LOCAL_UPLOADER="tools/esptool.py",
    LOCAL_UPLOADERFLAGS=[
       "--port", "$UPLOAD_PORT",
       "--baud", "$UPLOAD_SPEED",
       "write_flash", "0x00000",
    ],
    UPLOADCMD='$LOCAL_UPLOADER $LOCAL_UPLOADERFLAGS $SOURCE'
)
