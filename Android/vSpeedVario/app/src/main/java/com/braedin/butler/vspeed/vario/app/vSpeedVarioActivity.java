package com.braedin.butler.vspeed.vario.app;

import android.Manifest;
import android.app.Fragment;
import android.app.FragmentManager;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.ColorDrawable;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.media.AudioFormat;
import android.media.AudioManager;
import android.media.AudioTrack;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.provider.Settings;
import android.support.v4.app.ActivityCompat;
import android.text.Spannable;
import android.text.SpannableStringBuilder;
import android.text.style.ForegroundColorSpan;
import android.util.Log;
import android.util.TypedValue;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.SubMenu;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.EditorInfo;
import android.view.inputmethod.InputMethodManager;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import com.braedin.butler.vspeed.vario.R;
import com.braedin.butler.vspeed.vario.app.settings.ConnectedSettingsActivity;
import com.braedin.butler.vspeed.vario.ble.BleManager;
//import com.adafruit.bluefruit.le.connect.mqtt.MqttManager;
//import com.adafruit.bluefruit.le.connect.mqtt.MqttSettings;

//import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.text.DecimalFormat;
import java.util.ArrayList;


public class vSpeedVarioActivity extends vSpeedVarioInterfaceActivity /*implements MqttManager.MqttManagerListener*/ {

    double splitAlti = 0;
    double altiCalibrate = 0;
    int GPS_Altitude = 0;
    boolean pressureSync = false;

    double chartSpeed = 30;    // Seconds for completion; Live speed if zero;
    long chartMillis = 0;

    public double maxGforce = 1;
    public double netAcceleration = 9.81;
    public boolean sendAccel = false;
    public long accelMillis = 0;

    public int AVGprogressValue;
    public int CLIMBprogressValue;
    public int SINKprogressValue;
    //LIFT WIDGETS
    //public String liftWidget = "ThermalBearing";
    int[] veloColor = new int[]{
            // 0 == blue
            // 1 == orange
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0
    };
    int[] veloRadius = new int[]{
            // veloRadius[i] == veloAtBearing[i]
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0
    };
    public String liftWidget = "LineChart";
    //public String liftWidget = "ThermalCircle";
    public double[] Xcoords = new double[]{
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    public double[] Ycoords = new double[]{
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    public double[] longitudeToFeetZeroed = new double[]{
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    public double[] latitudeToFeetZeroed = new double[]{
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0,
            0,0,0,0,0,0,0,0,0,0
    };
    public double xRmax = 0;
    //public double xmin = 0;
    public double yRmax = 0;
    //public double ymin = 0;
    public double thermalRadius = 0;
    public Boolean flagThis = false;
    //private Boolean timeToSetGpsOrigin = true;

    //public String liftWidget = "SmileyFace";
    /*double[] smileyFaceY = new double[]{
            42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42,
            42, 42, 42, 42, 42, 42, 42, 42
    };*/

    //ALTITUDE WIDGETS
    public String altitudeWidget = "ScrollingAltitude";

    //BEEP
    public AudioTrack tone;
    //public CheckBox internalBeep;
    private double altitudeTriggerMemory;
    private long timeTriggerMemory;

    private long beepMillis = 0;
    private long toneMillis = 0;
    private boolean needBeeps = false;
    private double percentageOfCycleOn = 0.50;


    private boolean dbg = false;                // set true when debugging is needed
    private double verticalTrigger = 1.0;		// default feet
    private double sinkAlarm = -1.0;		    // default feet per second
    private double sinkAlarmDuration = 500;	    // default milliseconds
    private double sinkAlarmPitch = 200;	    // default Hz
    private double sap = sinkAlarmPitch;
    private double climbDurationShort = 50.0;	// default milliseconds
    private double climbDurationLong = 500.0;	// default milliseconds
    public double pitchMax = 500.0;             // default Hz
    public double pitchMin = 300.0;             // default Hz
    private double beepDuration = climbDurationLong;
    private double beepPitch = pitchMin;

    //GPS
    private CheckBox GPS;
    private TextView speedGPS;
    private TextView altitudeGPS;
    private TextView headingGPS;
    public float bearing;
    public double bearingPrev = 0.0;
    public long bearingMillisPrev = 0;
    public long bearingMillis;
    public double bearingRate;  // degrees per second
    public double prevBearingRate;
    public int[] veloAtBearing = new int[]{
            0,0,0,0,0,0,
            0,0,0,0,0,0,
            0,0,0,0,0,0,
            0,0,0,0,0,0,
            0,0,0,0,0,0,
            0,0,0,0,0,0
    };

    private LocationManager locationManager;
    private LocationListener locationListener;


    int chartHeight;
    int chartWidth;
    int[] lineChartY = new int[2000];/*{
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24,
            24, 24, 24, 24, 24, 24, 24, 24
    };*/
    boolean initChart = true;

    private String part1;
    private String part2;
    private boolean flagForPart2 = false;
    private int pass = 0;
    private int cnt = 0;

    private double previousMillis = 0;
    public boolean displaychecked = true;
    public boolean beepchecked = false;
    public boolean batterychecked = true;

    // Log
    private final static String TAG = vSpeedVarioActivity.class.getSimpleName();

    // Configuration
    private final static boolean kUseColorsForData = true;
    public final static int kDefaultMaxPacketsToPaintAsText = 500;

    // Activity request codes (used for onActivityResult)
    private static final int kActivityRequestCode_ConnectedSettingsActivity = 0;
    //private static final int kActivityRequestCode_MqttSettingsActivity = 1;

    // Constants
    private final static String kPreferences = "UartActivity_prefs";
    private final static String kPreferences_eol = "eol";
    //private final static String kPreferences_echo = "echo";
    private final static String kPreferences_asciiMode = "ascii";
    private final static String kPreferences_timestampDisplayMode = "timestampdisplaymode";

    // Colors
    private int mTxColor;
    private int mRxColor;
    private int mInfoColor = Color.parseColor("#F21625");

    // UI
    /*private EditText mBufferTextView;
    private ListView mBufferListView;
    private TimestampListAdapter mBufferListAdapter;*/
    private EditText mSendEditText;
    //private MenuItem mMqttMenuItem;
    //private Handler mMqttMenuItemAnimationHandler;
    //private TextView mSentBytesTextView;
    //private TextView mReceivedBytesTextView;

    // UI TextBuffer (refreshing the text buffer is managed with a timer because a lot of changes an arrive really fast and could stall the main thread)
    private Handler mUIRefreshTimerHandler = new Handler();
    private Runnable mUIRefreshTimerRunnable = new Runnable() {
        @Override
        public void run() {
            if (isUITimerRunning) {
                updateTextDataUI();
                // Log.d(TAG, "updateDataUI");
                mUIRefreshTimerHandler.postDelayed(this, 200);
            }
        }
    };
    private boolean isUITimerRunning = false;

    // Data
    private boolean mShowDataInHexFormat;
    private boolean mIsTimestampDisplayMode;
    //private boolean mIsEchoEnabled;
    private boolean mIsEolEnabled;
    private boolean mIsBeepEnabled = true;
    private boolean mIsBatteryDisplayed = true;
    private boolean mIsOledOn = true;

    private volatile SpannableStringBuilder mTextSpanBuffer;
    private volatile ArrayList<vSpeedVarioDataChunk> mDataBuffer;
    private volatile int mSentBytes;
    private volatile int mReceivedBytes;

    private DataFragment mRetainedDataFragment;

    private int maxPacketsToPaintAsText;

    public static final String PREFS_NAME = "prefsFile";
    public String velocityUnits = "VELO (ft/s)";
    public String altitudeUnits = "ALTI (ft)";
    public double veloMultiplier = 1;
    public double altiMultiplier = 1;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vspeedvario);

        SharedPreferences settings0 = getSharedPreferences(PREFS_NAME,0);
        SharedPreferences settings1 = getSharedPreferences(PREFS_NAME,1);
        SharedPreferences settings2 = getSharedPreferences(PREFS_NAME,2);

        velocityUnits = settings0.getString("veloUnits", velocityUnits);
        altitudeUnits = settings1.getString("altiUnits", altitudeUnits);
        maxGforce = round(settings2.getFloat("maxG", (float)maxGforce),1);

        TextView veloUnits = (TextView) findViewById(R.id.veloUnits);
        veloUnits.setText(velocityUnits);

        TextView altiUnits = (TextView) findViewById(R.id.altiUnits);
        altiUnits.setText(altitudeUnits);

        TextView maxG = (TextView) findViewById(R.id.maxGforce);
        maxG.setText("MAX G'S: ".concat(String.valueOf(maxGforce)));

        if(velocityUnits.equals("VELO (ft/s)")){veloMultiplier=1;}
        else if(velocityUnits.equals("VELO (m/s)")){veloMultiplier=0.3048;}
        else if(velocityUnits.equals("VELO (ft/min)")){veloMultiplier=60;}

        if(altitudeUnits.equals("ALTI (ft)")){altiMultiplier=1;}
        else if(altitudeUnits.equals("ALTI (m)")){altiMultiplier=0.3048;}



        averagingFilterSeekBar();
        climbThresholdSeekBar();
        sinkThresholdSeekBar();




        SensorManager sensorManager = (SensorManager) getSystemService(Context.SENSOR_SERVICE);
        Sensor sensor = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);

        sensorManager.registerListener(new SensorEventListener() {
            @Override
            public void onSensorChanged(SensorEvent event) {

                float x = event.values[0];
                float y = event.values[1];
                float z = event.values[2];
                netAcceleration = roundThreeDecimals( Math.sqrt(x * x + y * y + z * z) );
                if(netAcceleration/9.81 > maxGforce) {
                    maxGforce = round(netAcceleration/9.81, 1);

                    //TODO -- save max g force:
                    SharedPreferences settings2 = getSharedPreferences(PREFS_NAME,2);
                    SharedPreferences.Editor editor = settings2.edit();
                    editor.putFloat("maxG", (float)maxGforce);
                    editor.apply();

                    TextView gForce = (TextView) findViewById(R.id.maxGforce);
                    gForce.setText("max G's: ".concat(String.valueOf(maxGforce)));
                }


                //System.out.print("NetAcceleration==");System.out.println(netAcceleration);
                TextView accelStat = (TextView) findViewById(R.id.accelStat);


                //MUST SEND AT LEAST AS FAST AS SAMPLE RATE ON VARIO
                if(sendAccel && System.currentTimeMillis()-accelMillis >= 100){
                    accelMillis = System.currentTimeMillis();
                    vSpeedVarioSendData("x".concat(String.valueOf(netAcceleration)), false);
                    accelStat.setText("SENDING: x".concat(String.valueOf(netAcceleration)));
                }
                else if(!sendAccel){accelStat.setText(String.valueOf(netAcceleration));}
            }

            @Override
            public void onAccuracyChanged(Sensor sensor, int accuracy) {
            }

        }, sensor, SensorManager.SENSOR_DELAY_FASTEST);


        //timeToSetGpsOrigin = true;
        speedGPS = (TextView) findViewById(R.id.groundspeed);
        altitudeGPS = (TextView) findViewById(R.id.gpsaltitude);
        headingGPS = (TextView) findViewById(R.id.gpsheading);

        GPS = (CheckBox) findViewById(R.id.internalgps);

        locationManager = (LocationManager) getSystemService(LOCATION_SERVICE);

        locationListener = new LocationListener() {
            @Override
            public void onLocationChanged(Location location) {
                if(GPS.isChecked()) {
                    speedGPS.setVisibility(View.VISIBLE);
                    altitudeGPS.setVisibility(View.VISIBLE);
                    headingGPS.setVisibility(View.VISIBLE);

                    //COORDS
                    for (int i = 1; i < 30; i++) {
                        Xcoords[i - 1] = Xcoords[i];
                        Ycoords[i - 1] = Ycoords[i];
                        /*System.out.print(" Xcoords[");
                        System.out.print(i - 1);
                        System.out.print("] == ");
                        System.out.print(Xcoords[i - 1]);
                        System.out.print("   Ycoords[");
                        System.out.print(i - 1);
                        System.out.print("] == ");
                        System.out.println(Ycoords[i - 1]);*/
                    }
                    Xcoords[29] = location.getLongitude();
                    Ycoords[29] = location.getLatitude();
                    /*System.out.print(" Xcoords[");
                    System.out.print(29);
                    System.out.print("] == ");
                    System.out.print(Xcoords[29]);
                    System.out.print("   Ycoords[");
                    System.out.print(29);
                    System.out.print("] == ");
                    System.out.println(Ycoords[29]);*/


                    if(liftWidget.equals("ThermalCircle")) {
                        //23ft == 0.00102 - 0.00095 == 0.00007 longitude
                        //17ft == 0.000569 - 0.000475 == 0.000094 latitude
                        //    1ft               23ft
                        // -----------  =  ------------
                        //    xlong         0.00007long
                        // xlong = 0.00007 / 23
                        // = approx 0.00000304 long per 1 ft
                        // = approx 0.00000553 lat  per 1 ft

                        double xsum = 0;
                        for (int i = 0; i < 30; i++) {
                            xsum += Xcoords[i];
                        }
                        double averageLongitude = xsum / 30;

                        double ysum = 0;
                        for (int i = 0; i < 30; i++) {
                            ysum += Ycoords[i];
                        }
                        double averageLatitude = ysum / 30;

                        xRmax = 0;
                        yRmax = 0;

                        /*System.out.println(" ");
                        System.out.println(" ");
                        System.out.println("====== COORDS ======");*/

                        for (int i = 0; i < 30; i++) {
                            longitudeToFeetZeroed[i] = ((Xcoords[i] - (averageLongitude)) / 0.00000304);
                            latitudeToFeetZeroed[i] = ((Ycoords[i] - (averageLatitude)) / 0.00000553);

                            if (longitudeToFeetZeroed[i] > 45) {
                                longitudeToFeetZeroed[i] = 45;
                            } else if (longitudeToFeetZeroed[i] < -45) {
                                longitudeToFeetZeroed[i] = -45;
                            }

                            if (latitudeToFeetZeroed[i] > 45) {
                                latitudeToFeetZeroed[i] = 45;
                            } else if (latitudeToFeetZeroed[i] < -45) {
                                latitudeToFeetZeroed[i] = -45;
                            }

                            if (xRmax < Math.abs(longitudeToFeetZeroed[i])) {
                                xRmax = longitudeToFeetZeroed[i];
                            }
                            if (yRmax < Math.abs(latitudeToFeetZeroed[i])) {
                                yRmax = latitudeToFeetZeroed[i];
                            }

                            if (xRmax > yRmax) {
                                thermalRadius = xRmax;
                            } else if (xRmax < yRmax) {
                                thermalRadius = yRmax;
                            }

                            if (thermalRadius < 10) {
                                thermalRadius = 10;
                            }

                            //double newPixelX = adjustScale(longitudeToFeetZeroed[i], thermalRadius);

                            /*System.out.print(i);
                            System.out.print(" Xft==");
                            System.out.print(longitudeToFeetZeroed[i]);
                            System.out.print("   Yft==");
                            System.out.println(latitudeToFeetZeroed[i]);*/
                        }

                        /*System.out.print(" thermalRadius==");
                        System.out.println(thermalRadius);
                        System.out.println(" ");
                        System.out.println(" ");*/
                        // Summary: the greatest distance in any direction that a pixel travels will scale the radius of the image
                    }

                    TextView coords = (TextView) findViewById(R.id.coords);

                    GPS_Altitude = (int)Math.round(location.getAltitude()*3.28084);
                    String altitude;
                    if(altiMultiplier==0.3048){altitude = String.valueOf((int)(GPS_Altitude*altiMultiplier) + "m");}
                    else{altitude = String.valueOf((int)(GPS_Altitude*altiMultiplier) + "ft");}
                    String speed = String.valueOf(Math.round(location.getSpeed()*2.23694)).concat("mph");
                    altitudeGPS.setText(altitude);
                    speedGPS.setText(speed);


                    bearingMillis = System.currentTimeMillis();
                    bearing = location.getBearing();
                    if(liftWidget.equals("ThermalBearing")) {
                        bearingRate = (1000.0 * (bearing - bearingPrev)) / (bearingMillis - bearingMillisPrev);  //degrees per second
                        bearingPrev = bearing;
                        bearingMillisPrev = bearingMillis;
                        coords.setTextColor(Color.parseColor("#00ff00"));
                        coords.setText("     bearingRate:  "
                                .concat(String.valueOf(Math.round(bearingRate)))
                                .concat("    accuracy:  ")
                                .concat(String.valueOf(Math.round(location.getAccuracy()*3.28084))));
                        //System.out.print("  bearingRate==");
                        //System.out.println(bearingRate);
                    }
                    else{
                        coords.setTextColor(Color.parseColor("#ff0000"));
                        coords.setText(String.valueOf(Xcoords[29])
                                .concat(" ")
                                .concat(String.valueOf(Ycoords[29]))
                                .concat("  accuracy:")
                                .concat(String.valueOf(location.getAccuracy()*3.28084)));
                    }


                    //headingGPS.setText(String.valueOf(Math.round(bearing)));
                    String heading = "N";
                    if(bearing>337.5||bearing<=22.5){heading="N";}
                    else if(bearing>22.5&&bearing<=67.5){heading="NE";}
                    else if(bearing>67.5&&bearing<=112.5){heading="E";}
                    else if(bearing>112.5&&bearing<=157.5){heading="SE";}
                    else if(bearing>157.5&&bearing<=202.5){heading="S";}
                    else if(bearing>202.5&&bearing<=247.5){heading="SW";}
                    else if(bearing>247.5&&bearing<=292.5){heading="W";}
                    else if(bearing>292.5&&bearing<=337.5){heading="NW";}
                    //headingGPS.setText(heading);
                    headingGPS.setText(String.valueOf(Math.round(bearing)).concat("° ").concat(heading));
                    flagThis = true;
                }else if(!GPS.isChecked()){
                    speedGPS.setVisibility(View.INVISIBLE);
                    altitudeGPS.setVisibility(View.INVISIBLE);
                    headingGPS.setVisibility(View.INVISIBLE);
                }
            }

            @Override
            public void onStatusChanged(String s, int i, Bundle bundle) {

            }

            @Override
            public void onProviderEnabled(String s) {

            }

            @Override
            public void onProviderDisabled(String s) {
                Intent i = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
                startActivity(i);
            }
        };

        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_FINE_LOCATION) != PackageManager.PERMISSION_GRANTED && ActivityCompat.checkSelfPermission(this, Manifest.permission.ACCESS_COARSE_LOCATION) != PackageManager.PERMISSION_GRANTED) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.M) {
                requestPermissions(new String[]{Manifest.permission.ACCESS_COARSE_LOCATION,Manifest.permission.ACCESS_FINE_LOCATION,Manifest.permission.INTERNET}
                        ,10);
            }
            return;
        }
        if(GPS.isChecked()){
            /*speedGPS.setVisibility(View.VISIBLE);
            altitudeGPS.setVisibility(View.VISIBLE);
            headingGPS.setVisibility(View.VISIBLE);*/
            locationManager.requestLocationUpdates("gps", 1000, 0, locationListener);
        }else if(!GPS.isChecked()){
            /*speedGPS.setVisibility(View.INVISIBLE);
            altitudeGPS.setVisibility(View.INVISIBLE);
            headingGPS.setVisibility(View.INVISIBLE);*/
        }

        TextView climb = (TextView) findViewById(R.id.climb);
        TextView sink = (TextView) findViewById(R.id.sink);

        // Display
        //resetSettingsToDefaults();
        drawDisplay();
        //beepBasedOnAltitude();

        mBleManager = BleManager.getInstance(this);
        restoreRetainedDataFragment();

        // Get default theme colors
        TypedValue typedValue = new TypedValue();
        Resources.Theme theme = getTheme();
        theme.resolveAttribute(R.attr.colorPrimaryDark, typedValue, true);
        mTxColor = typedValue.data;
        theme.resolveAttribute(R.attr.colorControlActivated, typedValue, true);
        mRxColor = typedValue.data;


        mSendEditText = (EditText) findViewById(R.id.sendEditText);
        mSendEditText.setOnEditorActionListener(new TextView.OnEditorActionListener() {
            @Override
            public boolean onEditorAction(TextView textView, int actionId, KeyEvent keyEvent) {
                if (actionId == EditorInfo.IME_ACTION_SEND) {
                    //mSendEditText.clearFocus();
                    onClickSend(null);
                    return true;
                }
                if(actionId==EditorInfo.IME_ACTION_DONE){
                    //Clear focus here from edittext
                    mSendEditText.clearFocus();
                }

                return false;
            }
        });
        mSendEditText.setOnFocusChangeListener(new View.OnFocusChangeListener() {
            public void onFocusChange(View view, boolean hasFocus) {
                if (!hasFocus) {
                    // Dismiss keyboard when sendEditText loses focus
                    dismissKeyboard(view);
                    //showKeyboard(view);

                }
            }
        });


        // Read shared preferences
        maxPacketsToPaintAsText = 1;/*PreferencesFragment.getUartTextMaxPackets(this);*/
        //Log.d(TAG, "maxPacketsToPaintAsText: "+maxPacketsToPaintAsText);

        // Read local preferences
        SharedPreferences preferences = getSharedPreferences(kPreferences, MODE_PRIVATE);
        mShowDataInHexFormat = !preferences.getBoolean(kPreferences_asciiMode, true);
        final boolean isTimestampDisplayMode = true;/*preferences.getBoolean(kPreferences_timestampDisplayMode, false);*/
        setDisplayFormatToTimestamp(isTimestampDisplayMode);
        //mIsEchoEnabled = false;/*preferences.getBoolean(kPreferences_echo, true);*/
        mIsEolEnabled = false;/*preferences.getBoolean(kPreferences_eol, true);*/
        invalidateOptionsMenu();        // udpate options menu with current values

        // Continue
        onServicesDiscovered();

    }

    @Override
    public void onResume() {
        super.onResume();

        // Setup listeners
        mBleManager.setBleListener(this);

        // Start UI refresh
        //Log.d(TAG, "add ui timer");
        updateUI();

        isUITimerRunning = true;
        mUIRefreshTimerHandler.postDelayed(mUIRefreshTimerRunnable, 0);

    }

    @Override
    public void onPause() {
        super.onPause();

        //Log.d(TAG, "remove ui timer");
        isUITimerRunning = false;
        mUIRefreshTimerHandler.removeCallbacksAndMessages(mUIRefreshTimerRunnable);

        // Save preferences
        SharedPreferences preferences = getSharedPreferences(kPreferences, MODE_PRIVATE);
        SharedPreferences.Editor editor = preferences.edit();
        //editor.putBoolean(kPreferences_echo, mIsEchoEnabled);
        editor.putBoolean(kPreferences_eol, mIsEolEnabled);
        editor.putBoolean(kPreferences_asciiMode, !mShowDataInHexFormat);
        editor.putBoolean(kPreferences_timestampDisplayMode, mIsTimestampDisplayMode);

        editor.apply();
    }

    @Override
    public void onDestroy() {

        // Retain data
        saveRetainedDataFragment();

        super.onDestroy();
    }

    public double adjustScale(double pixelMagnitude, double pixMagnitudeMax){
        double upperLeftPixel = -45;
        double lowerRightPixel = 45;
        double rMin = -1 * pixMagnitudeMax;
        return (((lowerRightPixel - upperLeftPixel) / (pixMagnitudeMax - rMin)) * (pixelMagnitude - rMin)) + upperLeftPixel;
    }

    public void dismissKeyboard(View view) {
        InputMethodManager imm = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
        imm.hideSoftInputFromWindow(view.getWindowToken(), 0);
        //imm.showSoftInput(mSendEditText, InputMethodManager.SHOW_IMPLICIT);
    }

    /*public void onMenuItemSelected(MenuItem beep) {
        String data = "b";
        vSpeedVarioSendData(data, false);
    }*/

    public void onClickSend(View view) {
        String data = mSendEditText.getText().toString();
        mSendEditText.setText("");       // Clear editText

        vSpeedVarioSendData(data, false);
    }

    private AudioTrack generateTone(double freqHz, int durationMs)
    {
        int count = (int)(44100.0 * 2.0 * (durationMs / 1000.0)) & ~1;
        short[] samples = new short[count];
        for(int i = 0; i < count; i += 2){
            short sample = (short)(Math.sin(2 * Math.PI * i / (44100.0 / freqHz)) * 0x7FFF);
            samples[i + 0] = sample;
            samples[i + 1] = sample;
        }
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 44100,
                AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT,
                count * (Short.SIZE / 8), AudioTrack.MODE_STATIC);
        track.write(samples, 0, count);
        return track;
    }

    private AudioTrack generateGradientTone(double freqHzStart, double freqHzEnd, int durationMs)
    {
        int count = (int)(44100.0 * 2.0 * (durationMs / 1000.0)) & ~1;
        short[] samples = new short[count];
        double[] freqHz = new double[count];
        for(int i = 0; i < count; i++){
            freqHz[i] = (((freqHzEnd - freqHzStart) / (count - 0)) * (i - 0)) + freqHzStart;
        }
        //double freqHz = freqHzStart;
        for(int i = 0; i < count; i += 2){
            short sample = (short)(Math.sin(2 * Math.PI * i / (44100.0 / freqHz[i])) * 0x7FFF);
            samples[i + 0] = sample;
            samples[i + 1] = sample;
            //freqHz+=5;
        }
        AudioTrack track = new AudioTrack(AudioManager.STREAM_MUSIC, 44100,
                AudioFormat.CHANNEL_OUT_STEREO, AudioFormat.ENCODING_PCM_16BIT,
                count * (Short.SIZE / 8), AudioTrack.MODE_STATIC);
        track.write(samples, 0, count);
        return track;
    }

    double prevSplitAlti = 0;
    double prevSplitVelo = 0;
    double prevSplitVoltage = 0;
    String prevIncoming = "a(4912.23)av(-0.12)vb(V)b";
    String prevFormattedData = "0_0_V";
    String prevSV = "0V";


    //TODO -- SINK ALARM VARIABLE PITCH
    public void beepBasedOnAltitude(double currentAltitude, long currentTime){
        if(currentTime-timeTriggerMemory<100){return;}
        //AudioTrack tone = generateTone(440, 250);
        //tone.play();

        /* (DECISION 1) Has there been a positive altitude change great enough to trigger a beep? */
        if(currentAltitude - altitudeTriggerMemory >= verticalTrigger)
        {
            if(dbg){
                System.out.print(" [D1Y] *BEEP* ");
                System.out.print(" a:");System.out.print(currentAltitude - altitudeTriggerMemory);
                System.out.print(" ");
            }

		/* (DECISION 2) Is the duration of the beep going to be too long? */
            if(((currentTime - timeTriggerMemory) / 2.0) > climbDurationLong)
            {
                if(dbg) {
                    System.out.print(" [D2Y] ");
                    System.out.print(" t:"); System.out.print(currentTime-timeTriggerMemory);
                    System.out.print(" ");
                }
                beepDuration = (int) climbDurationLong; // Limit the beep duration
            }
            else{
                beepDuration = (int) ((currentTime - timeTriggerMemory) / 2.0); // Don't limit the beep duration

                if(dbg) {
                    System.out.print(" [D2N] ");
                    System.out.print(" t:"); System.out.print(currentTime-timeTriggerMemory);
                    System.out.print(" ");
                }
            }

            // Determine pitch by mapping the values based on beepDuration
            //beepPitch = (beepDuration - climbDurationLong) * (pitchMax - pitchMin)/(climbDurationShort - climbDurationLong) + pitchMin;
            beepPitch = Math.round((((pitchMax - pitchMin) / (climbDurationShort - climbDurationLong)) * (beepDuration - climbDurationLong)) + pitchMin);

            if(dbg){
                System.out.print(" d:"); System.out.print(beepDuration);
                System.out.print(" p:"); System.out.print(beepPitch);
                System.out.print(" ");
            }
            //tone(buzzerPin, beepPitch, beepDuration+(0.25*beepDuration));   // Activate the beep

            //TODO -- Prevent app crash
            //tone.getState();
            try{
                if(tone.getPlayState() == 3){
                    tone.pause();
                    //tone.flush();
                }
                //tone.stop();
                //tone.release();
            }
            catch (NullPointerException a){}
            catch(IllegalStateException a){}
            beepDuration = Math.round(beepDuration+(0.25*beepDuration));
            if(beepDuration<20){beepDuration=20;}
            if(beepPitch>pitchMax){beepPitch=pitchMax;}
            tone = generateTone(beepPitch, (int)beepDuration);
            try{tone.play();}
            catch (IllegalStateException a){}

            altitudeTriggerMemory = Math.round(currentAltitude);    // Use currentAltitude as the next reference point
            timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
        }
        else{
            if(dbg) {System.out.print(" [D1N] ");}

		/* (DECISION 3) Has there been a negative altitude change great enough to trigger the sinkAlarm?  */
            if(altitudeTriggerMemory - currentAltitude >= verticalTrigger)
            {
                if(dbg) {
                    System.out.print(" [D3Y] a:");
                    System.out.print(altitudeTriggerMemory - currentAltitude);
                }

			/* (DECISION 4) is the altitude dropping fast enough to trigger the sinkAlarm?  */
                if((1000.0*(currentAltitude - altitudeTriggerMemory)) / (currentTime - timeTriggerMemory) <= sinkAlarm)
                {
                    if(dbg) {
                        System.out.print(" [D4Y] *SINK*");
                        System.out.print(" d:"); System.out.print(sinkAlarmDuration);
                        System.out.print(" p:"); System.out.print(sinkAlarmPitch);
                    }
                    //tone(buzzerPin, sinkAlarmPitch, sinkAlarmDuration); // initiate sinkAlarm

                    //TODO -- Prevent app crash
                    try{
                        if(tone.getPlayState() == 3){
                            tone.pause();
                            //tone.flush();
                        }
                        //tone.stop();
                        //tone.release();
                    }
                    catch (NullPointerException a){}
                    catch(IllegalStateException a){}
                    tone = generateTone(sinkAlarmPitch, (int)sinkAlarmDuration);
                    try{tone.play();}
                    catch (IllegalStateException a){}
                    //System.out.println(AudioTrack.SUCCESS);  <-- ??? I don't remember what I was doing here

                    altitudeTriggerMemory = Math.round(currentAltitude);            // Use currentAltitude as the next reference point
                    timeTriggerMemory = currentTime;                    // Use currentTime as the next reference point
                }
                else{
                    if(dbg) {System.out.print(" [D4N] ");}
                    altitudeTriggerMemory = Math.round(currentAltitude);    // Use currentAltitude as the next reference point
                    timeTriggerMemory = currentTime;            // Use currentTime as the next reference point
                }
            }
            else{
                if(dbg) {System.out.print(" [D3N] ");}
            }
        }

        //TODO -- DEBUG AUDIO: display pitch and duration; catch it when it crashes the Audio Service;
        TextView audioStat = (TextView)findViewById(R.id.audiostat);
        audioStat.setText("Cp[".concat(String.valueOf(beepPitch)).concat("] Cd[").concat(String.valueOf(beepDuration)).concat("]"));
        //System.out.print("Cp[".concat(String.valueOf(beepPitch)).concat("] Cd[").concat(String.valueOf(beepDuration)).concat("]"));

        TextView sinkStat = (TextView)findViewById(R.id.sinkstat);
        sinkStat.setText(" Sp[".concat(String.valueOf(sinkAlarmPitch)).concat("] Sd[").concat(String.valueOf(sinkAlarmDuration)).concat("]"));
        //System.out.println(" Sp[".concat(String.valueOf(sinkAlarmPitch)).concat("] Sd[").concat(String.valueOf(sinkAlarmDuration)).concat("]"));

        //vSpeedVarioSendData(String.valueOf(netAcceleration), false);
    }




    public void beepBasedOnVelocity(double currentAltitude, double velo, long currentTime){

        //Prevents audio crashing ...hopefully you'll never need more than 20 beeps per second...
        if(currentTime - /*beepMillis*/toneMillis < 100){return;}
        else{toneMillis = currentTime;}

        //OPTION[1] BEEP DURATION DEPENDS VELOCITY
        //beepDuration = (((climbDurationShort - climbDurationLong) / (5.0 - 0.0)) * (velo - 0.0)) + climbDurationLong;

        //DON'T LET THE DURATION RUN FOR TOO LONG
        //if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}

        //BEEP PITCH DEPENDS ON VELOCITY
        beepPitch = (((pitchMax - pitchMin) / (10.0 - 0.0)) * (velo - 0.0)) + pitchMin;

        //ALLOW A BEEP IF CLIMBED MORE THAN A CERTAIN AMOUNT
        if(currentAltitude - altitudeTriggerMemory >= verticalTrigger){

            //TELL THE BUZZER TO EITHER START OR CONTINUE TO BEEP
            needBeeps = true;

            //OPTION[2] CALCULATE BEEP DURATION BASED ON TIME SINCE LAST BEEP
            beepDuration = (((float)currentTime - (float)timeTriggerMemory) / 2.0);

            //DON'T LET THE DURATION RUN FOR TOO LONG
            if(beepDuration > climbDurationLong){beepDuration = climbDurationLong;}

            //RESET THE NEXT CLIMB BEEP TRIGGER REFERENCE POINT
            altitudeTriggerMemory = currentAltitude;

            //USE CURRENT TIME AS NEXT REFERENCE POINT FOR DETERMINING BEEP DURATION
            timeTriggerMemory = currentTime;

        }

        //RESET THE NEXT CLIMB TRIGGER'S REFERENCE POINT, AND INITIATE SINK ALARM IF SINKING
        else if(currentAltitude - altitudeTriggerMemory < 0){altitudeTriggerMemory = currentAltitude;}

        //IF BEEP SHOULD BE BEEPING, EITHER START OR CONTINUE TO BEEP
        if(needBeeps /*&& currentTime-toneMillis>100*/){

            //tone(buzzerPin, beepPitch, (percentageOfCycleOn+0.5)*beepDuration);

            try{if(tone.getPlayState() == 3){tone.pause();/*tone.stop();tone.release();*/}}
            catch (NullPointerException a){}
            catch(IllegalStateException a){}
            tone = generateTone(beepPitch, (int)((percentageOfCycleOn+0.5)*beepDuration));
            try{tone.play();}
            catch (IllegalStateException a){}
            //toneMillis = currentTime;
        }

        //PRINT DEBUG INFO
        //Serial.print("velo:"); Serial.print(velo); Serial.print(" needBeeps:"); Serial.print(needBeeps); Serial.print(" beepDuration:"); Serial.print(beepDuration); Serial.print(" beepPitch:"); Serial.println(beepPitch);

        //LET THE BUZZER KNOW THAT IT HAS BEEPED LONG ENOUGH
        if(currentTime - beepMillis >= beepDuration){needBeeps = false; beepMillis = currentTime;}

        //IF SINKING FASTER THAN SPECIFIED, INITIATE SINK ALARM
        if(velo <= sinkAlarm && currentTime-toneMillis>100){

            //tone(buzzerPin, beepPitch, sinkAlarmDuration);

            try{if(tone.getPlayState() == 3){tone.pause();/*tone.stop();tone.release();*/}}
            catch (NullPointerException a){}
            catch(IllegalStateException a){}
            tone = generateTone(beepPitch, (int)(sinkAlarmDuration));
            try{tone.play();}
            catch (IllegalStateException a){}
            //toneMillis = currentTime;
        }
        //TODO -- DEBUG AUDIO: display pitch and duration; catch it when it crashes the Audio Service;
        TextView audioStat = (TextView)findViewById(R.id.audiostat);
        audioStat.setText("Cp[".concat(String.valueOf(beepPitch)).concat("] Cd[").concat(String.valueOf(beepDuration)).concat("]"));
        //System.out.print("Cp[".concat(String.valueOf(beepPitch)).concat("] Cd[").concat(String.valueOf(beepDuration)).concat("]"));

        TextView sinkStat = (TextView)findViewById(R.id.sinkstat);
        sinkStat.setText(" Sp[".concat(String.valueOf(sinkAlarmPitch)).concat("] Sd[").concat(String.valueOf(sinkAlarmDuration)).concat("]"));
        //System.out.println(" Sp[".concat(String.valueOf(sinkAlarmPitch)).concat("] Sd[").concat(String.valueOf(sinkAlarmDuration)).concat("]"));

        //toneMillis = currentTime;
    }


    private static double round (double value, int precision) {
        int scale = (int) Math.pow(10, precision);
        return (double) Math.round(value * scale) / scale;
    }

    public void maxGclick(View view){
        maxGforce = 0;
        Context context = getApplicationContext();
        CharSequence text = "G-Force Reset";
        int duration = Toast.LENGTH_SHORT;

        Toast toast = Toast.makeText(context, text, duration);
        toast.show();
    }

    public void onTapChangeLiftWidget(View view){
        //sendAccel = !sendAccel;
        /*if(liftWidget.equals("LineChart")){liftWidget = "ThermalCircle";}
        else if(liftWidget.equals("ThermalCircle")){liftWidget = "ThermalBearing";}
        else if(liftWidget.equals("ThermalBearing")){liftWidget = "SmileyFace";}
        else if(liftWidget.equals("SmileyFace")){liftWidget = "LineChart";}*/
        System.out.print(" sendAccel=="); System.out.println(sendAccel);
    }

    public void onInternalBeepClick(View view){
        CheckBox intBeep = (CheckBox) findViewById(R.id.internalbeep);
        int toneDuration = 250;
        if(intBeep.isChecked()){
            tone = generateGradientTone(pitchMin, pitchMax, toneDuration);
            if(tone.getPlayState() == 3){tone.stop();}
            tone.play();
            while(((double)tone.getPlaybackHeadPosition()/(double)tone.getSampleRate())*1000.0 < toneDuration){
                /*wait until tone finishes*/
            }
        }
        else if(!intBeep.isChecked()){
            tone = generateGradientTone(pitchMax, pitchMin, toneDuration);
            if(tone.getPlayState() == 3){tone.stop();}
            tone.play();
            while(((double)tone.getPlaybackHeadPosition()/(double)tone.getSampleRate())*1000.0 < toneDuration){
                /*wait until tone finishes*/
            }
        }
    }

    //TODO -- drawDisplay()
    public void drawDisplay(){

        //vSpeedVarioSendData(String.valueOf(netAcceleration), false);


        TextView incomingText = (TextView) findViewById(R.id.altitudeFt);
        String incoming = incomingText.getText().toString().concat("_CRC");
        /*//if incoming text contains two decimal points
        if(incoming.indexOf("." ,(incoming.indexOf(".")) ) > 0){
            incoming = prevIncoming;
        }
        prevIncoming = incoming;*/
        //splitAlti;
        double splitVelo;
        double splitVoltage;
        String CRC = "_CRC";
        String sV = "0V";
        String[] splitText = incoming.split("_");
        ProgressBar battery = (ProgressBar) findViewById(R.id.vbat);

        /*try{
            //if a battery voltage value isn't being transmitted...
            if(splitText[2].equals("V")){
                sV = "0V";
                battery.setVisibility(View.INVISIBLE);
            }else{
                sV = splitText[2];
                battery.setVisibility(View.VISIBLE);
            }
        }
        catch(ArrayIndexOutOfBoundsException a){
            System.out.print(" sV_EXCEPTION: ");
            sV = prevSV;
            if(sV.equals("0V")){
                battery.setVisibility(View.INVISIBLE);
            }else{
                battery.setVisibility(View.VISIBLE);
            }
        }
        prevSV = sV;*/



        try{splitAlti = Double.valueOf(splitText[0])-altiCalibrate;}
        catch (Exception a) {/*System.out.print(" EXCEPTION[a]: ");*/splitAlti = prevSplitAlti;}

        try{splitVelo = Double.valueOf(splitText[1]);}
        catch (Exception b) {/*System.out.print(" EXCEPTION[b]: ");*/splitVelo = prevSplitVelo;}

        try{splitVoltage = Double.valueOf(splitText[2].replace("V",""));}
        catch (Exception c) {/*System.out.print(" EXCEPTION[c]: ");*/splitVoltage = prevSplitVoltage;}

        try{if(!CRC.equals("_"+splitText[3])){CRC = "_FAILED";}}
        catch (Exception d) {/*System.out.print(" EXCEPTION[d]: ");*/CRC = "_ERROR";}

        if(splitVoltage == 0){
            battery.setVisibility(View.INVISIBLE);
        }else{battery.setVisibility(View.VISIBLE);}

        int batteryPercent = (int)(splitVoltage*156.25 - 556.25);
        battery.setProgress(batteryPercent);
        battery.setMinimumWidth(20);
        CheckBox batt = (CheckBox) findViewById(R.id.battery);
        if(batteryPercent<=0){
            batt.setText("BATT:_");
        }
        else{batt.setText("BATT: ".concat(String.valueOf(batteryPercent)).concat("%"));}

        //is it really this hard to change the color of the progress bar???
        if(batteryPercent<33){battery.getProgressDrawable().setColorFilter(Color.parseColor("#ff0000"), android.graphics.PorterDuff.Mode.SRC_ATOP);}
        else if(batteryPercent<66){battery.getProgressDrawable().setColorFilter(Color.parseColor("#ffff00"), android.graphics.PorterDuff.Mode.SRC_ATOP);}
        else{battery.getProgressDrawable().setColorFilter(Color.parseColor("#ffffff"), android.graphics.PorterDuff.Mode.SRC_ATOP);}

        /*System.out.print(" incoming:"); System.out.println(incoming);
        System.out.print(" splits:  "); System.out.print(splitAlti);
        System.out.print("   "); System.out.print(splitVelo);
        System.out.print("   "); System.out.print(splitVoltage);
        System.out.print("   CRC:"); System.out.println(CRC);
        System.out.println(" ");*/

        //long currentMillis = SystemClock.currentThreadTimeMillis();

        CheckBox internalBeep = (CheckBox) findViewById(R.id.internalbeep);
        if(internalBeep.isChecked()){
            beepBasedOnAltitude(splitAlti, System.currentTimeMillis());
            //beepBasedOnVelocity(splitAlti, splitVelo, System.currentTimeMillis());
        }


        //== SCROLLING ALTITUDE ==============================================/
        if(altitudeWidget.equals("ScrollingAltitude")) {
            splitAlti*=altiMultiplier;
            long roundedAlti = Math.round(splitAlti);
            TextView splitAltitude = (TextView) findViewById(R.id.splitAltitude);
            splitAltitude.setText(String.valueOf(roundedAlti));

            TextView borderAltitude = (TextView) findViewById(R.id.border);
            borderAltitude.setText(String.valueOf(roundedAlti));

            TextView measureParent = (TextView) findViewById(R.id.measureparent);
            int parentHeight = measureParent.getHeight();
            int ownHeight = splitAltitude.getHeight();

            TextView altitop = (TextView) findViewById(R.id.topAltitude);
            altitop.setText(String.valueOf(Math.round(roundedAlti + 0.5)));
            altitop.setTranslationY((float) ((ownHeight + parentHeight / 2) * (splitAlti - (int) splitAlti)) - (ownHeight + parentHeight / 2));

            TextView altibottom = (TextView) findViewById(R.id.bottomAltitude);
            altibottom.setText(String.valueOf(Math.round(roundedAlti - 0.5)));
            altibottom.setTranslationY((float) ((ownHeight + parentHeight / 2) * (splitAlti - (int) splitAlti)));

            float transY = (float) ((ownHeight + parentHeight / 2) * (splitAlti - (int) splitAlti));

            /*System.out.print(" parentHeight:");System.out.print(parentHeight);
            System.out.print(" ownHeight:");System.out.print(ownHeight);
            System.out.print(" splitAlti:");System.out.print(splitAlti);
            System.out.print(" transY:");System.out.println(transY);*/
        }
        //====================================================================/


        //int velo = (int) (((splitAlti-prevSplitAlti))*(splitSamples));
        double velo = splitVelo;

        /*System.out.println(" ");
        System.out.print("velo==");
        System.out.println(velo);*/

        int separate = 5;
        if(velo <= sinkAlarm-6){sinkAlarmPitch = sap - separate*6;}
        else if(velo <= sinkAlarm-5){sinkAlarmPitch = sap - separate*5;}
        else if(velo <= sinkAlarm-4){sinkAlarmPitch = sap - separate*4;}
        else if(velo <= sinkAlarm-3){sinkAlarmPitch = sap - separate*3;}
        else if(velo <= sinkAlarm-2){sinkAlarmPitch = sap - separate*2;}
        else if(velo <= sinkAlarm-1){sinkAlarmPitch = sap - separate;}
        else{sinkAlarmPitch = sap;}

        //System.out.print(" sinkAlarmPitch == ");System.out.println(sinkAlarmPitch);

        //System.out.print(" incoming:");System.out.print(incoming);
        //System.out.print(" diffAlti:");System.out.print(splitAlti-prevSplitAlti);
        //System.out.print(" currentMillis:");System.out.print(currentMillis);
        //System.out.print(" velo:");System.out.println(velo);

        prevSplitAlti = splitAlti;
        //previousMillis = currentMillis;
        prevSplitVoltage = splitVoltage;
        prevSplitVelo = splitVelo;

        TextView velocity = (TextView) findViewById(R.id.velocity);
        velocity.setText(String.valueOf((int)(velo*veloMultiplier)));



        //== LINE CHART ===================================================/
        FrameLayout fl = (FrameLayout) findViewById(R.id.chart);
        TextView measureParent = (TextView) findViewById(R.id.measureparent);
        if(liftWidget.equals("LineChart")) {

            Paint black = new Paint();
            black.setColor(Color.parseColor("#000000"));

            Paint white = new Paint();
            white.setColor(Color.parseColor("#ffffff"));

            chartHeight = measureParent.getHeight()/2;
            chartWidth = measureParent.getWidth()/2;
            if(chartHeight<=0){chartHeight=1;}
            if(chartWidth<=0){chartWidth=1;}
            //System.out.println("HEIGHT="+chartHeight+" WIDTH="+chartWidth);

            if(initChart&&chartHeight>1) {
                for(int i=0; i<lineChartY.length; i++){
                    lineChartY[i] = chartHeight / 2;
                }
                initChart=false;
            }

            double divider = 5.3;
            //POSITIVE AXIS LABELS:
            TextView pos10 = (TextView) findViewById(R.id.pos10);
            //pos10.setText(10 * (int)veloMultiplier);

            TextView pos8 = (TextView) findViewById(R.id.pos8);
            pos8.setTranslationY((float)(1*chartHeight/divider));

            TextView pos6 = (TextView) findViewById(R.id.pos6);
            pos6.setTranslationY((float)(2*chartHeight/divider));

            TextView pos4 = (TextView) findViewById(R.id.pos4);
            pos4.setTranslationY((float)(3*chartHeight/divider));

            TextView pos2 = (TextView) findViewById(R.id.pos2);
            pos2.setTranslationY((float)(4*chartHeight/divider));

            //NEGATIVE AXIS LABELS:
            TextView neg2 = (TextView) findViewById(R.id.neg2);
            neg2.setTranslationY((float)(6*chartHeight/divider));

            TextView neg4 = (TextView) findViewById(R.id.neg4);
            neg4.setTranslationY((float)(7*chartHeight/divider));

            TextView neg6 = (TextView) findViewById(R.id.neg6);
            neg6.setTranslationY((float)(8*chartHeight/divider));

            TextView neg8 = (TextView) findViewById(R.id.neg8);
            neg8.setTranslationY((float)(9*chartHeight/divider));

            Bitmap bg = Bitmap.createBitmap(chartWidth, chartHeight, Bitmap.Config./*ALPHA_8*/ARGB_8888);

            Canvas canvas = new Canvas(bg);
            canvas.drawRect(0, 0, chartWidth, chartHeight, black);

            //IF ITS TIME TO SHIFT THE LINE CHART
            if(System.currentTimeMillis()-chartMillis >= chartSpeed*1000.0/chartWidth) {
                chartMillis = System.currentTimeMillis();
                int p = -1 * (int) (velo/*veloMultiplier*/*(chartHeight/20)) + (int)(chartHeight/2.0);
                for (int i = 0; i < chartWidth-1; i++) {
                    lineChartY[i] = lineChartY[i + 1];        // Shift all pixels to the left one
                    //canvas.drawPoint(i, lineChartY[i], white);  // Draw all the new pixels except the most recent
                    canvas.drawLine(i, lineChartY[i], i + 1, lineChartY[i + 1], white);
                }
                lineChartY[chartWidth-1] = p;
                if (lineChartY[chartWidth-1] > chartHeight-1) {
                    lineChartY[chartWidth-1] = chartHeight-1;
                } else if (lineChartY[chartWidth-1] < 0) {
                    lineChartY[chartWidth-1] = 0;
                }
            }
            //OTHERWISE, EVERYTHING STAYS THE SAME
            else{
                for(int i=0; i<chartWidth-1; i++){
                    canvas.drawLine(i, lineChartY[i], i + 1, lineChartY[i + 1], white);
                }
            }

            canvas.drawPoint(chartWidth-1, lineChartY[chartWidth-1], white);  // Draw the most recent pixel
            for (int i = 0; i < chartWidth-5; i += 4) {
                canvas.drawPoint(i, (int)(chartHeight/2.0), white);
            }

            fl.setBackground(new BitmapDrawable(bg));
        }
        //====================================================================/


        //== THERMAL CIRCLE ==================================================/
        LinearLayout thermalParent = (LinearLayout) findViewById(R.id.thermalparent);
        //int parentHeight = thermalParent.getHeight();
        //int parentWidth = thermalParent.getWidth();
        TextView thermalCanvas = (TextView) findViewById(R.id.canvasforthermal);

        if(liftWidget.equals("ThermalCircle") || liftWidget.equals("ThermalBearing")){
            fl.setBackground(new ColorDrawable(Color.parseColor("#000000")));
            thermalCanvas.setVisibility(View.VISIBLE);

            if(thermalParent.getHeight() > thermalParent.getWidth()){
                // draw a square the size of the width
                thermalCanvas.setHeight(thermalParent.getWidth());
                thermalCanvas.setWidth(thermalParent.getWidth());
            }
            else if(thermalParent.getHeight() < thermalParent.getWidth()){
                // draw a square the size of the height
                thermalCanvas.setHeight(thermalParent.getHeight());
                thermalCanvas.setWidth(thermalParent.getHeight());
            }
            else{
                // leave the size alone
            }

            Paint black = new Paint();
            black.setColor(Color.parseColor("#000000"));

            Paint white = new Paint();
            white.setColor(Color.parseColor("#ffffff"));

            /*Paint outline = new Paint();
            outline.setColor(Color.parseColor("#ffffff"));*/

            Paint prettyBlue = new Paint();
            prettyBlue.setColor(Color.parseColor("#00ffff"));

            Bitmap bg = Bitmap.createBitmap(100, 100, Bitmap.Config.ARGB_8888);

            Canvas canvas = new Canvas(bg);
            if(liftWidget.equals("ThermalCircle")) {
                canvas.drawRect(0, 0, 100, 100, white);
                canvas.drawRect(1, 1, 99, 99, black);
            }

            if(liftWidget.equals("ThermalBearing")){
                Boolean turningRight = true;
                float radialPosition;
                if(bearingRate >= 0) {
                    turningRight = true;
                    radialPosition = bearing;
                }
                else{
                    turningRight = false;
                    radialPosition = bearing + 180;
                    if(radialPosition >= 360) {
                        radialPosition -= 360;
                    }
                }

                canvas.save();
                canvas.rotate(radialPosition, 50, 50);

                canvas.drawCircle(50,50,36,prettyBlue);     //Circle
                canvas.drawCircle(50,50,35,black);          //Fill

                if(turningRight) {
                    canvas.drawRect(10, 46, 20, 56, black);         //Spacing
                    canvas.drawLine(9, 50, 19, 50, prettyBlue);     //Wings
                    canvas.drawLine(14, 48, 14, 54, prettyBlue);    //Fuselage
                    canvas.drawLine(12, 53, 16, 53, prettyBlue);    //Elevator
                }
                else{
                    canvas.drawRect(10, 44, 20, 54, black);         //Spacing
                    canvas.drawLine(9, 50, 19, 50, prettyBlue);     //Wings
                    canvas.drawLine(14, 52, 14, 46, prettyBlue);    //Fuselage
                    canvas.drawLine(12, 47, 16, 47, prettyBlue);    //Elevator
                }

                canvas.restore();

                //CLEAR THE VALUES OF veloAtBearing[] IF bearingRate INDICATES A TURN IN THE OTHER DIRECTION
                if((bearingRate>=0 && prevBearingRate<0) || (bearingRate<=0 && prevBearingRate>0)){
                    for(int i = 0; i < 36; i++){
                        veloAtBearing[i] = 0;
                    }
                }
                prevBearingRate = bearingRate;

                //UPDATE THE RADIUS OF NEW VELO DOT
                veloAtBearing[(int) (radialPosition / 10.0)] = (int)velo;

                for(int degrees = 0; degrees < 36; degrees++) {
                    canvas.save();  // smash all canvas layers together
                    canvas.rotate(degrees * 10, 50, 50);    // add a layer and rotate it 10 degrees for next velo dot

                    //TELL THE VELO DOT WHAT COLOR IT SHOULD BE SET TO
                    if (veloAtBearing[degrees] > 0) {
                        veloColor[degrees] = 1;   // Set to Orange if climbing
                    } else {
                        veloColor[degrees] = 0;   // Set to Blue if not climbing
                    }

                    //SET THE COLOR OF NEW VELO DOT
                    Paint vColor = new Paint();
                    switch (veloColor[degrees]) {
                        case 1:
                            vColor.setColor(Color.parseColor("#ff9900"));   // Orange
                            break;
                        default:
                            vColor.setColor(Color.parseColor("#0000ff"));   // Blue
                            break;
                    }
                    //now we know the color based on the magnitude of veloAtBearing[i]

                    //DRAW COLORED VELO DOTS
                    canvas.drawCircle(14, 50, Math.abs(veloAtBearing[degrees]), vColor);  // draw on the rotated layer
                    canvas.restore();   // "un-rotate" the layer you've just drawn
                }
            }

            if(liftWidget.equals("ThermalCircle")) {
                canvas.drawCircle(50, 50, (float) thermalRadius, prettyBlue);
                canvas.drawCircle(50, 50, (float) thermalRadius - 1, black);
                for (int i = 0; i <= 100; i += 10) {
                    canvas.drawPoint(i, 50, white);
                }
                for (int i = 0; i <= 100; i += 10) {
                    canvas.drawPoint(50, i, white);
                }

                double newPixelX;
                double newPixelY;
                for (int i = 1; i < 29; i++) {
                    //canvas.drawPoint((int)longitudeToFeetZeroed[i]+50, (int)latitudeToFeetZeroed[i]+50, white);
                    double prevPixelX = adjustScale(longitudeToFeetZeroed[i - 1], thermalRadius) + 50;
                    newPixelX = adjustScale(longitudeToFeetZeroed[i], thermalRadius) + 50;
                    double prevPixelY = adjustScale(latitudeToFeetZeroed[i - 1], thermalRadius) + 50;
                    newPixelY = adjustScale(latitudeToFeetZeroed[i], thermalRadius) + 50;
                    //canvas.drawPoint((int)newPixelX, (int)newPixelY, white);
                    canvas.drawLine((float) prevPixelX, (float) prevPixelY, (float) newPixelX, (float) newPixelY, white);
                }
                //canvas.drawPoint((int)longitudeToFeetZeroed[29]+50, (int)latitudeToFeetZeroed[29]+50, prettyBlue);   // THIS DOT IS ME!
                newPixelX = adjustScale(longitudeToFeetZeroed[29], thermalRadius) + 50;
                newPixelY = adjustScale(latitudeToFeetZeroed[29], thermalRadius) + 50;
                //canvas.drawPoint((int)newPixelX, (int)newPixelY, prettyBlue);
                canvas.drawCircle((float) newPixelX, (float) newPixelY, 3, prettyBlue);

                //canvas.rotate(bearing);    <--??? I don't see a difference

                if (flagThis) {
                    flagThis = false;
                    System.out.print("   newPixelX==");
                    System.out.print(newPixelX);
                    System.out.print("  newPixelY==");
                    System.out.println(newPixelY);
                }
            }

            thermalCanvas.setBackground(new BitmapDrawable(bg));
        }
        else{
            thermalCanvas.setVisibility(View.INVISIBLE);
        }
        //====================================================================/



        //== THERMAL BEARING =================================================/
        /*if(liftWidget.equals("ThermalBearing")){

        }
        else{

        }*/
        //====================================================================/



        //== SMILEY FACE =====================================================/
        if(liftWidget.equals("SmileyFace")){
            double smileyMax = 2517;
            double smileyMin = -2517;
            double yPixelMin = 0;
            double yPixelMax = 63;
            //int xPixelMin = 0;
            //int xPixelMax = 64;

            Paint black = new Paint();
            black.setColor(Color.parseColor("#000000"));

            Paint white = new Paint();
            white.setColor(Color.parseColor("#ffffff"));

            Paint yellow = new Paint();
            yellow.setColor(Color.parseColor("#ffff00"));

            Bitmap bg = Bitmap.createBitmap(64, 64, Bitmap.Config.ARGB_8888);

            Canvas canvas = new Canvas(bg);
            canvas.drawRect(0, 0, 64, 64, black);

            canvas.drawPoint(42, 11, yellow);
            canvas.drawPoint(40, 11, yellow);
            canvas.drawPoint(41, 10, yellow);
            canvas.drawPoint(41, 12, yellow);
            canvas.drawPoint(55, 11, yellow);
            canvas.drawPoint(54, 12, yellow);
            canvas.drawPoint(54, 10, yellow);
            canvas.drawPoint(53, 11, yellow);

            //System.out.println("== SMILEY FACE  ==");
            for(int i = 2; i < 30; i++){
                //smileyFaceY[i] = (int) Math.round((((yPixelMax - yPixelMin) / (smileyMax - smileyMin)) * ( ((/*-1**/(velo)*(i-48)^2)+43) - smileyMin)) + yPixelMin);
                double smileyY = (-1*(velo)*Math.pow(( i -16),2)+43);
                //smileyFaceY[i] = ((((yPixelMax - yPixelMin) / (smileyMax - smileyMin)) * (smileyY - smileyMin)) + yPixelMin);
                double smileyFaceY = ((((yPixelMax - yPixelMin) / (smileyMax - smileyMin)) * (smileyY - smileyMin)) + yPixelMin);
                float sfy = (float) smileyFaceY;

                canvas.drawPoint(i+32, sfy, yellow);
                //System.out.print(" sfy[");System.out.print(i+32);System.out.print("] == ");
                //System.out.println(sfy);
            }
            //System.out.println("==================");

            /*for (int i = 0; i < 16; i += 1) {
                canvas.drawPoint(i+32, smileyFaceY[i], yellow);
            }*/

            //FrameLayout fl = (FrameLayout) findViewById(R.id.chart);
            fl.setBackground(new BitmapDrawable(bg));
        }
        //====================================================================/
    }

    public double roundThreeDecimals(double d)
    {
        DecimalFormat threeDForm = new DecimalFormat("#.###");
        return Double.valueOf(threeDForm.format(d));
    }

    public void averagingFilterSeekBar(){
        SeekBar avgFilter = ( SeekBar ) findViewById(R.id.avgfilter);

        avgFilter.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener(){

                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        AVGprogressValue = i;
                        AVGprogressValue = 10 * AVGprogressValue;
                        if(AVGprogressValue >1000){AVGprogressValue = 1000;}
                        String sendAsFilter = "a";
                        vSpeedVarioSendData(sendAsFilter.concat(String.valueOf(AVGprogressValue)), false);
                        TextView avgFilterValue = ( TextView ) findViewById(R.id.averaging);
                        String base = "AVG: ";
                        avgFilterValue.setText(base.concat(String.valueOf(AVGprogressValue)).concat(" ms"));
                        System.out.print("   avgFilterValue==");
                        System.out.println(AVGprogressValue);
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        if(AVGprogressValue >1000){AVGprogressValue = 1000;}
                        System.out.print("avgFilter.getProgress()==");
                        System.out.print(AVGprogressValue);
                        String sendAsFilter = "a";
                        vSpeedVarioSendData(sendAsFilter.concat(String.valueOf(AVGprogressValue)), false);
                        TextView avgFilterValue = ( TextView ) findViewById(R.id.averaging);
                        String base = "AVG: ";
                        avgFilterValue.setText(base.concat(String.valueOf(AVGprogressValue)).concat(" ms"));
                        System.out.print("   avgFilterValue==");
                        System.out.println(AVGprogressValue);
                    }
                }
        );
    }


    //TODO--
    public void switchVeloUnits(View view){
        TextView veloUnits = (TextView) findViewById(R.id.veloUnits);
        if(velocityUnits.equals("VELO (ft/s)")){velocityUnits = "VELO (m/s)"; veloMultiplier=0.3048;}
        else if(velocityUnits.equals("VELO (m/s)")){velocityUnits = "VELO (ft/min)"; veloMultiplier=60;}
        else if(velocityUnits.equals("VELO (ft/min)")){velocityUnits = "VELO (ft/s)"; veloMultiplier=1;}

        SharedPreferences settings0 = getSharedPreferences(PREFS_NAME,0);
        SharedPreferences.Editor editor = settings0.edit();
        editor.putString("veloUnits", velocityUnits);
        editor.apply();

        veloUnits.setText(velocityUnits);
    }

    //TODO--
    public void switchAltiUnits(View view){
        TextView altiUnits = (TextView) findViewById(R.id.altiUnits);
        if(altitudeUnits.equals("ALTI (ft)")){altitudeUnits = "ALTI (m)"; altiMultiplier=0.3048;}
        else if(altitudeUnits.equals("ALTI (m)")){altitudeUnits = "ALTI (ft)"; altiMultiplier=1;}

        SharedPreferences settings1 = getSharedPreferences(PREFS_NAME,1);
        SharedPreferences.Editor editor = settings1.edit();
        editor.putString("altiUnits", altitudeUnits);
        editor.apply();

        altiUnits.setText(altitudeUnits);
    }


    public void climbThresholdSeekBar(){
        SeekBar climbThreshold = ( SeekBar ) findViewById(R.id.ClimbThreshold);

        climbThreshold.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener(){

                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        CLIMBprogressValue = i;
                        double ClimbProgressValue = (((4) / (100.0)) * (CLIMBprogressValue)) + 1;
                        CLIMBprogressValue = (int)Math.round(ClimbProgressValue);

                        String sendAsClimb = "ct";
                        vSpeedVarioSendData(sendAsClimb.concat(String.valueOf(CLIMBprogressValue)), false);
                        TextView climbValue = ( TextView ) findViewById(R.id.climbtrigger);
                        String base = "CLIMB: ";
                        climbValue.setText(base.concat(String.valueOf(CLIMBprogressValue)).concat(" ft"));
                        verticalTrigger = CLIMBprogressValue;
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        String sendAsClimb = "ct";
                        vSpeedVarioSendData(sendAsClimb.concat(String.valueOf(CLIMBprogressValue)), false);
                        TextView climbValue = ( TextView ) findViewById(R.id.climbtrigger);
                        String base = "CLIMB: ";
                        climbValue.setText(base.concat(String.valueOf(CLIMBprogressValue)).concat(" ft"));
                        verticalTrigger = CLIMBprogressValue;
                    }
                }
        );
    }


    public void sinkThresholdSeekBar(){
        SeekBar sinkThreshold = ( SeekBar ) findViewById(R.id.SinkThreshold);

        sinkThreshold.setOnSeekBarChangeListener(
                new SeekBar.OnSeekBarChangeListener(){

                    @Override
                    public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                        SINKprogressValue = i;
                        double SinkProgressValue = (((9) / (100.0)) * (SINKprogressValue)) -10;
                        SINKprogressValue = (int)Math.round(SinkProgressValue);

                        String sendAsSink = "st";
                        vSpeedVarioSendData(sendAsSink.concat(String.valueOf(SINKprogressValue)), false);
                        TextView sinkValue = ( TextView ) findViewById(R.id.sinktrigger);
                        String base = "SINK: ";
                        sinkValue.setText(base.concat(String.valueOf(SINKprogressValue)).concat(" ft/s"));
                        sinkAlarm = SINKprogressValue;
                    }

                    @Override
                    public void onStartTrackingTouch(SeekBar seekBar) {

                    }

                    @Override
                    public void onStopTrackingTouch(SeekBar seekBar) {
                        String sendAsSink = "st";
                        vSpeedVarioSendData(sendAsSink.concat(String.valueOf(SINKprogressValue)), false);
                        TextView sinkValue = ( TextView ) findViewById(R.id.sinktrigger);
                        String base = "SINK: ";
                        sinkValue.setText(base.concat(String.valueOf(SINKprogressValue)).concat(" ft/s"));
                        sinkAlarm = SINKprogressValue;
                    }
                }
        );
    }


    public void calibrateAltitude(View view){
        altiCalibrate = splitAlti - GPS_Altitude;

        Context context = getApplicationContext();
        pressureSync = !pressureSync;
        CharSequence text;
        if(pressureSync) {
            text = "Pressure & GPS Synced";
        }
        else{
            text = "Pressure & GPS NOT Synced";
        }
        int duration = Toast.LENGTH_SHORT;
        Toast toast = Toast.makeText(context, text, duration);
        toast.show();
    }


    public void displayclick(View view){
        CheckBox display = ( CheckBox ) findViewById(R.id.display);
        if(!displaychecked){
            displaychecked = true;
            display.setChecked(true);
            vSpeedVarioSendData("D", false);
        }else{
            displaychecked = false;
            display.setChecked(false);
            vSpeedVarioSendData("d", false);
        }
    }

    public void beepclick(View view){
        CheckBox beep = ( CheckBox ) findViewById(R.id.beep);
        if(!beepchecked){
            beepchecked = true;
            beep.setChecked(true);
            vSpeedVarioSendData("B", false);
        }else{
            beepchecked = false;
            beep.setChecked(false);
            vSpeedVarioSendData("b", false);
        }
    }


    public void batteryclick(View view){
        CheckBox battery = ( CheckBox ) findViewById(R.id.battery);
        if(!batterychecked){
            battery.setChecked(true);
            batterychecked = true;
            vSpeedVarioSendData("V", false);
        }else{
            battery.setChecked(false);
            batterychecked = false;
            vSpeedVarioSendData("v", false);
        }
    }

    private void vSpeedVarioSendData(String data, boolean wasReceivedFromMqtt) {

        // Add eol
        /*if (mIsEolEnabled) {
            // Add newline character if checked
            data += "\n";
        }*/

        // Allow Vario Beep
        /*Boolean justBarely = true;
        if (mIsBeepEnabled && justBarely) {
            justBarely = false;
            data = "b";
            vSpeedVarioSendData(data, false);
        }*/

        // Send to uart
        sendData(data);
        mSentBytes += data.length();


        // Add to current buffer
        byte[] bytes = new byte[0];
        try {
            bytes = data.getBytes("UTF-8");
        } catch (UnsupportedEncodingException e) {
            e.printStackTrace();
        }
        vSpeedVarioDataChunk dataChunk = new vSpeedVarioDataChunk(System.currentTimeMillis(), vSpeedVarioDataChunk.TRANSFERMODE_TX, bytes);
        mDataBuffer.add(dataChunk);

        final String formattedData = mShowDataInHexFormat ? bytesToHex(bytes) : bytesToText(bytes, true);
        //if (mIsTimestampDisplayMode) {
            //final String currentDateTimeString = DateFormat.getTimeInstance().format(new Date(dataChunk.getTimestamp()));
            //mBufferListAdapter.add(new TimestampData(/*"[" + currentDateTimeString + "] TX: " + */formattedData, mTxColor));
            //mBufferListView.setSelection(mBufferListAdapter.getCount());
        //}



        // Update UI
        updateUI();
    }



    private void setDisplayFormatToTimestamp(boolean enabled) {
        mIsTimestampDisplayMode = true;/*enabled;*/
        /*mBufferTextView.setVisibility(enabled ? View.GONE : View.VISIBLE);
        mBufferListView.setVisibility(enabled ? View.VISIBLE : View.GONE);*/
    }

    // region Menu
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_vspeedvario, menu);


        // DisplayMode
        MenuItem displayModeMenuItem = menu.findItem(R.id.action_displaymode);
        displayModeMenuItem.setTitle(String.format(getString(R.string.vspeedvario_action_displaymode_format), getString(/*mIsTimestampDisplayMode ? */R.string.vspeedvario_displaymode_updatingvalue/* : R.string.vspeedvario_displaymode_text*/)));
        SubMenu displayModeSubMenu = displayModeMenuItem.getSubMenu();
        if (mIsTimestampDisplayMode) {
            MenuItem displayModeTimestampMenuItem = displayModeSubMenu.findItem(R.id.action_displaymode_timestamp);
            displayModeTimestampMenuItem.setChecked(true);
        } else {
            MenuItem displayModeTextMenuItem = displayModeSubMenu.findItem(R.id.action_displaymode_text);
            displayModeTextMenuItem.setChecked(true);
        }

        // DataMode
        MenuItem dataModeMenuItem = menu.findItem(R.id.action_datamode);
        dataModeMenuItem.setTitle(String.format(getString(R.string.vspeedvario_action_datamode_format), getString(mShowDataInHexFormat ? R.string.vspeedvario_format_hexadecimal : R.string.vspeedvario_format_ascii)));
        SubMenu dataModeSubMenu = dataModeMenuItem.getSubMenu();
        if (mShowDataInHexFormat) {
            MenuItem dataModeHexMenuItem = dataModeSubMenu.findItem(R.id.action_datamode_hex);
            dataModeHexMenuItem.setChecked(true);
        } else {
            MenuItem dataModeAsciiMenuItem = dataModeSubMenu.findItem(R.id.action_datamode_ascii);
            dataModeAsciiMenuItem.setChecked(true);
        }

        // Echo
        /*MenuItem echoMenuItem = menu.findItem(R.id.action_echo);
        echoMenuItem.setTitle(R.string.vspeedvario_action_echo);
        echoMenuItem.setChecked(mIsEchoEnabled);*/

        // Eol
        MenuItem eolMenuItem = menu.findItem(R.id.action_eol);
        eolMenuItem.setTitle(R.string.vspeedvario_action_eol);
        eolMenuItem.setChecked(mIsEolEnabled);

        // BEEP
        MenuItem beepMenuItem = menu.findItem(R.id.action_beep);
        beepMenuItem.setTitle(R.string.vspeedvario_action_beep);
        beepMenuItem.setChecked(mIsBeepEnabled);

        // BATTERY DISPLAYED
        MenuItem batteryMenuItem = menu.findItem(R.id.action_displaybattery);
        batteryMenuItem.setTitle(R.string.vspeedvario_action_displaybattery);
        batteryMenuItem.setChecked(mIsBatteryDisplayed);

        // OLED DISPLAY
        MenuItem oledMenuItem = menu.findItem(R.id.action_oled);
        oledMenuItem.setTitle(R.string.vspeedvario_action_oled);
        oledMenuItem.setChecked(mIsOledOn);

        return true;
    }



    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        final int id = item.getItemId();

        switch (id) {
            case R.id.action_help:
                startHelp();
                return true;

            case R.id.action_connected_settings:
                startConnectedSettings();
                return true;

            case R.id.action_refreshcache:
                if (mBleManager != null) {
                    mBleManager.refreshDeviceCache();
                }
                break;

            case R.id.action_beep:
                mIsBeepEnabled = !mIsBeepEnabled;
                vSpeedVarioSendData("b", false);
                invalidateOptionsMenu();
                return true;

            case R.id.action_displaybattery:
                mIsBatteryDisplayed = !mIsBatteryDisplayed;
                vSpeedVarioSendData("v", false);
                invalidateOptionsMenu();
                return true;

            case R.id.action_oled:
                mIsOledOn = !mIsOledOn;
                vSpeedVarioSendData("d", false);
                invalidateOptionsMenu();
                return true;

            /*case R.id.action_displaymode_timestamp:
                setDisplayFormatToTimestamp(true);
                recreateDataView();
                invalidateOptionsMenu();
                return true;*/

            /*case R.id.action_displaymode_text:
                setDisplayFormatToTimestamp(false);
                recreateDataView();
                invalidateOptionsMenu();
                return true;*/

            /*case R.id.action_datamode_hex:
                mShowDataInHexFormat = true;
                recreateDataView();
                invalidateOptionsMenu();
                return true;*/

            /*case R.id.action_datamode_ascii:
                mShowDataInHexFormat = false;
                recreateDataView();
                invalidateOptionsMenu();
                return true;*/

            /*case R.id.action_echo:
                mIsEchoEnabled = !mIsEchoEnabled;
                invalidateOptionsMenu();
                return true;*/

            /*case R.id.action_eol:
                mIsEolEnabled = !mIsEolEnabled;
                invalidateOptionsMenu();
                return true;*/
        }

        return super.onOptionsItemSelected(item);
    }

    private void startConnectedSettings() {
        // Launch connected settings activity
        Intent intent = new Intent(this, ConnectedSettingsActivity.class);
        startActivityForResult(intent, kActivityRequestCode_ConnectedSettingsActivity);
    }

    @Override
    protected void onActivityResult(final int requestCode, final int resultCode, final Intent intent) {
        if (requestCode == kActivityRequestCode_ConnectedSettingsActivity && resultCode == RESULT_OK) {
            finish();
        }
    }

    private void startHelp() {
        // Launch app help activity
        Intent intent = new Intent(this, CommonHelpActivity.class);
        intent.putExtra("title", getString(R.string.vspeedvario_help_title));
        intent.putExtra("help", "uart_help.html");
        startActivity(intent);
    }
    // endregion

    // region BleManagerListener
    /*
    @Override
    public void onConnected() {

    }

    @Override
    public void onConnecting() {

    }
*/
    @Override
    public void onDisconnected() {
        super.onDisconnected();
        Log.d(TAG, "Disconnected. Back to previous activity");
        finish();
    }

    @Override
    public void onServicesDiscovered() {
        super.onServicesDiscovered();
        enableRxNotifications();
    }

    @Override
    public synchronized void onDataAvailable(BluetoothGattCharacteristic characteristic) {
        super.onDataAvailable(characteristic);
        // UART RX
        if (characteristic.getService().getUuid().toString().equalsIgnoreCase(UUID_SERVICE)) {
            if (characteristic.getUuid().toString().equalsIgnoreCase(UUID_RX)) {
                final byte[] bytes = characteristic.getValue();
                mReceivedBytes = bytes.length;

                final vSpeedVarioDataChunk dataChunk = new vSpeedVarioDataChunk(System.currentTimeMillis(), vSpeedVarioDataChunk.TRANSFERMODE_RX, bytes);
                mDataBuffer.add(dataChunk);

                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        //if (mIsTimestampDisplayMode) {
                            //final String currentDateTimeString = DateFormat.getTimeInstance().format(new Date(dataChunk.getTimestamp()));
                            /*final*/ String formattedData = mShowDataInHexFormat ? bytesToHex(bytes) : bytesToText(bytes, true);

                            //mBufferListAdapter.add(new TimestampData(/*"[" + currentDateTimeString + "] RX: " +*/ formattedData, mRxColor));
                            //mBufferListAdapter.add("[" + currentDateTimeString + "] RX: " + formattedData);
                            //mBufferListView.smoothScrollToPosition(mBufferListAdapter.getCount() - 1);
                            //mBufferListView.setSelection(mBufferListAdapter.getCount());

                        TextView altitudeFt = (TextView) findViewById(R.id.altitudeFt);
                        //System.out.print(" formattedData:");System.out.print(formattedData);

                        // 'V' is the char representing transmission completion...
                        if(cnt==0 && formattedData.charAt(formattedData.length()-1) != 'V'){
                            //received data is not complete
                            cnt = 1; //permission to handle the situation
                        }
                        else if(cnt == 0){//data is complete
                            altitudeFt.setText(formattedData);
                            prevFormattedData = formattedData;
                            cnt = 0; //permission to remain on course
                        }
                        else if(cnt == 1){//ignore first half of broken part
                            formattedData = prevFormattedData;
                            altitudeFt.setText(formattedData);
                            cnt = 2; //permission to enter next handler
                        }
                        else if(cnt == 2){//ignore second half of broken part
                            formattedData = prevFormattedData;
                            altitudeFt.setText(formattedData);
                            cnt = 0; //permission to exit handler
                        }

                        //System.out.println(formattedData);

                        //TRYING TO SALVAGE THE BROKEN PIECES INSTEAD OF IGNORING THEM...
                        /*if(!flagForPart2 && formattedData.charAt(formattedData.length()-1) != 'V'){
                            part1 = formattedData;
                            flagForPart2 = true;
                            illTakeTheNextPass = 1;
                        }else if(!flagForPart2){
                            altitudeFt.setText(formattedData);
                        }

                        if(illTakeTheNextPass==2 && flagForPart2){
                            illTakeTheNextPass = 0;
                            flagForPart2 = false;
                            part2 = formattedData;
                            String splicedData = part1.concat(part2);
                            //System.out.print(" part1:");System.out.print(part1);
                            //System.out.print(" part2:");System.out.print(part2);
                            //System.out.print(" splicedData:");System.out.print(splicedData);
                            altitudeFt.setText(splicedData);
                        }else if(illTakeTheNextPass==1 && flagForPart2){
                            illTakeTheNextPass = 2;
                        }*/

                        //}
                        updateUI();
                        drawDisplay();
                        //beepBasedOnAltitude();
                    }
                });

            }
        }
    }

    private String bytesToText(byte[] bytes, boolean simplifyNewLine) {
        String text = new String(bytes, Charset.forName("UTF-8"));
        if (simplifyNewLine) {
            text = text.replaceAll("(\\r\\n|\\r)", "\n");
        }
        return text;
    }
/*
    @Override
    public void onDataAvailable(BluetoothGattDescriptor descriptor) {

    }

    @Override
    public void onReadRemoteRssi(int rssi) {

    }
    */
    // endregion

    private void addTextToSpanBuffer(SpannableStringBuilder spanBuffer, String text, int color) {

        if (kUseColorsForData) {
            final int from = spanBuffer.length();
            spanBuffer.append(text);
            spanBuffer.setSpan(new ForegroundColorSpan(color), from, from + text.length(), Spannable.SPAN_EXCLUSIVE_EXCLUSIVE);
        } else {
            spanBuffer.append(text);
        }
    }

    private void updateUI() {
        //mSentBytesTextView.setText(String.format(getString(R.string.vspeedvario_sentbytes_format), mSentBytes));
        //mReceivedBytesTextView.setText(String.format(getString(R.string.vspeedvario_receivedbytes_format), mReceivedBytes));
    }


    private int mDataBufferLastSize = 0;

    private void updateTextDataUI() {

        if (!mIsTimestampDisplayMode) {
            if (mDataBufferLastSize != mDataBuffer.size()) {

                final int bufferSize = mDataBuffer.size();
                if (bufferSize > maxPacketsToPaintAsText) {
                    mDataBufferLastSize = bufferSize - maxPacketsToPaintAsText;
                    mTextSpanBuffer.clear();
                    addTextToSpanBuffer(mTextSpanBuffer, getString(R.string.uart_text_dataomitted) + "\n", mInfoColor);
                }

                // Log.d(TAG, "update packets: "+(bufferSize-mDataBufferLastSize));
                for (int i = mDataBufferLastSize; i < bufferSize; i++) {
                    final vSpeedVarioDataChunk dataChunk = mDataBuffer.get(i);
                    final boolean isRX = dataChunk.getMode() == vSpeedVarioDataChunk.TRANSFERMODE_RX;
                    final byte[] bytes = dataChunk.getData();
                    final String formattedData = mShowDataInHexFormat ? bytesToHex(bytes) : bytesToText(bytes, true);
                    addTextToSpanBuffer(mTextSpanBuffer, formattedData, isRX ? mRxColor : mTxColor);
                }

                mDataBufferLastSize = mDataBuffer.size();
                /*mBufferTextView.setText(mTextSpanBuffer);
                mBufferTextView.setSelection(0, mTextSpanBuffer.length());  */      // to automatically scroll to the end
            }
        }
    }

    private void recreateDataView() {

        if (mIsTimestampDisplayMode) {
            /*mBufferListAdapter.clear();*/

            final int bufferSize = mDataBuffer.size();
            for (int i = 0; i < bufferSize; i++) {

                final vSpeedVarioDataChunk dataChunk = mDataBuffer.get(i);
                final boolean isRX = dataChunk.getMode() == vSpeedVarioDataChunk.TRANSFERMODE_RX;
                final byte[] bytes = dataChunk.getData();
                final String formattedData = mShowDataInHexFormat ? bytesToHex(bytes) : bytesToText(bytes, true);

                //final String currentDateTimeString = DateFormat.getTimeInstance().format(new Date(dataChunk.getTimestamp()));
                //mBufferListAdapter.add(new TimestampData(/*"[" + currentDateTimeString + "] " + (isRX ? "RX" : "TX") + ": " +*/ formattedData, isRX ? mRxColor : mTxColor));
            }
            /*mBufferListView.setSelection(mBufferListAdapter.getCount());*/
        } else {
            mDataBufferLastSize = 0;
            mTextSpanBuffer.clear();
            /*mBufferTextView.setText("");*/
        }
    }

    private String bytesToHex(byte[] bytes) {
        StringBuilder stringBuffer = new StringBuilder();
        for (byte aByte : bytes) {
            String charString = String.format("%02X", (byte) aByte);

            stringBuffer.append(charString).append(" ");
        }
        return stringBuffer.toString();
    }

    // region DataFragment
    public static class DataFragment extends Fragment {
        private boolean mShowDataInHexFormat;
        private SpannableStringBuilder mTextSpanBuffer;
        private ArrayList<vSpeedVarioDataChunk> mDataBuffer;
        private int mSentBytes;
        private int mReceivedBytes;

        @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            setRetainInstance(true);
        }
    }

    private void restoreRetainedDataFragment() {
        // find the retained fragment
        FragmentManager fm = getFragmentManager();
        mRetainedDataFragment = (DataFragment) fm.findFragmentByTag(TAG);

        if (mRetainedDataFragment == null) {
            // Create
            mRetainedDataFragment = new DataFragment();
            fm.beginTransaction().add(mRetainedDataFragment, TAG).commit();

            mDataBuffer = new ArrayList<>();
            mTextSpanBuffer = new SpannableStringBuilder();
        } else {
            // Restore status
            mShowDataInHexFormat = mRetainedDataFragment.mShowDataInHexFormat;
            mTextSpanBuffer = mRetainedDataFragment.mTextSpanBuffer;
            mDataBuffer = mRetainedDataFragment.mDataBuffer;
            mSentBytes = mRetainedDataFragment.mSentBytes;
            mReceivedBytes = mRetainedDataFragment.mReceivedBytes;
        }
    }

    private void saveRetainedDataFragment() {
        mRetainedDataFragment.mShowDataInHexFormat = mShowDataInHexFormat;
        mRetainedDataFragment.mTextSpanBuffer = mTextSpanBuffer;
        mRetainedDataFragment.mDataBuffer = mDataBuffer;
        mRetainedDataFragment.mSentBytes = mSentBytes;
        mRetainedDataFragment.mReceivedBytes = mReceivedBytes;
    }
    // endregion



    // region TimestampAdapter
    private class TimestampData {
        String text;
        int textColor;

        TimestampData(String text, int textColor) {
            this.text = text;
            this.textColor = textColor;
        }
    }

    private class TimestampListAdapter extends ArrayAdapter<TimestampData> {

        TimestampListAdapter(Context context, int textViewResourceId) {
            super(context, textViewResourceId);
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            if (convertView == null) {
                convertView = LayoutInflater.from(getContext()).inflate(R.layout.layout_vspeedvario_datachunkitem, parent, false);
            }

            TimestampData data = getItem(position);
            TextView textView = (TextView) convertView;
            textView.setText(data.text);
            textView.setTextColor(data.textColor);

            return convertView;
        }
    }
    // endregion
}
