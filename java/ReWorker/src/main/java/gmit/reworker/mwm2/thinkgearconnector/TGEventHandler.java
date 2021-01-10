package gmit.reworker.mwm2.thinkgearconnector;

public interface TGEventHandler {
    void onBlink(int strength);

    void onScan(int poorSignalLevel, String status);

    void onESense(int attention, int meditation, int delta, int theta, int lowAlpha, int highAlpha, int lowBeta, int highBeta, int lowGamma, int highGamma, int poorSignalLevel);

    void onRawEeg(int eegLevel);
}
