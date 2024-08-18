#!/bin/sh

RADAE_PATH=$1
RADAE_VENV=$2

# The below does the following:
#     * For each block of OTA audio from freedv-gui:
#         * Convert the 8 kHz audio into IQ data via zero-padding.
#         * Pass the IQ data into the RADAE decoder 
#         * Send the resulting 16 kHz audio back to freedv-gui for playback.
#
# Note: Current RADAE scripts seem to require being executed from
# the RADAE folder.
cd $RADAE_PATH
export PATH=$RADAE_VENV/bin:$PATH
stdbuf -i0 -o0 python3 int16tof32.py --zeropad | \
stdbuf -i0 -o0 python3 radae_rx.py model19_check3/checkpoints/checkpoint_epoch_100.pth -v 2 --auxdata | \
stdbuf -i0 -o0 build/src/lpcnet_demo -fargan-synthesis - -
