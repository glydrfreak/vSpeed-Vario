package com.adafruit.bluefruit.le.connect.app;

class vSpeedVarioDataChunk {
    static final int TRANSFERMODE_TX = 0;
    static final int TRANSFERMODE_RX = 1;

    private long mTimestamp;        // in millis
    private int mMode;
    private byte[] mData;

    vSpeedVarioDataChunk(long timestamp, int mode, byte[] bytes) {
        mTimestamp = timestamp;
        mMode = mode;
        mData = bytes;
    }

    long getTimestamp() {
        return mTimestamp;
    }

    public int getMode() {
        return mMode;
    }

    public byte[] getData() {
        return mData;
    }
}
