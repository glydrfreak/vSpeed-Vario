package com.adafruit.bluefruit.le.connect.app;

import android.app.AlertDialog;
import android.app.Fragment;
import android.app.FragmentManager;
import android.bluetooth.BluetoothGattCharacteristic;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Rect;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.os.Handler;
import android.os.SystemClock;
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
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.TextView;

import com.adafruit.bluefruit.le.connect.R;
import com.adafruit.bluefruit.le.connect.app.settings.ConnectedSettingsActivity;
import com.adafruit.bluefruit.le.connect.app.settings.PreferencesFragment;
import com.adafruit.bluefruit.le.connect.ble.BleManager;
//import com.adafruit.bluefruit.le.connect.mqtt.MqttManager;
//import com.adafruit.bluefruit.le.connect.mqtt.MqttSettings;

//import org.eclipse.paho.client.mqttv3.MqttMessage;

import java.io.UnsupportedEncodingException;
import java.nio.charset.Charset;
import java.text.DateFormat;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Date;
import java.util.Objects;
import java.util.regex.Pattern;


public class vSpeedVarioActivity extends vSpeedVarioInterfaceActivity /*implements MqttManager.MqttManagerListener*/ {

    int[] y = new int[]{
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24,
            24,24,24,24,24,24,24,24
    };

    private String part1;
    private String part2;
    private boolean flagForPart2 = false;
    private int pass = 0;
    private int cnt = 0;

    private double previousMillis = 0;
    public boolean displaychecked = true;
    public boolean beepchecked = true;
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


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_vspeedvario);

        //resetSettingsToDefaults();
        drawDisplay();

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
/*

incoming     = "a(4912.23)av(0)vb(0.0)b"
splitText[0] = "a(4912.23)a"
splitText[0] = "v(0)v"
splitText[0] = "b(0.0)b"

 */
    double prevSplitAlti = 0;
    int prevSplitVelo = 0;
    double prevSplitVoltage = 0;
    String prevIncoming = "a(4912.23)av(0)vb(V)b";
    String prevFormattedData = "0_0_V";
    String prevSV = "0V";

    public void drawDisplay(){
        //TODO -- drawDisplay()
        TextView incomingText = (TextView) findViewById(R.id.altitudeFt);
        String incoming = incomingText.getText().toString().concat("_CRC");
        /*//if incoming text contains two decimal points
        if(incoming.indexOf("." ,(incoming.indexOf(".")) ) > 0){
            incoming = prevIncoming;
        }
        prevIncoming = incoming;*/
        double splitAlti;
        int splitVelo;
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



        try{splitAlti = Double.valueOf(splitText[0]);}
        catch (Exception a) {System.out.print(" EXCEPTION[a]: ");splitAlti = prevSplitAlti;}

        try{splitVelo = Integer.valueOf(splitText[1]);}
        catch (Exception b) {System.out.print(" EXCEPTION[b]: ");splitVelo = prevSplitVelo;}

        try{splitVoltage = Double.valueOf(splitText[2].replace("V",""));}
        catch (Exception c) {System.out.print(" EXCEPTION[c]: ");splitVoltage = prevSplitVoltage;}

        try{if(!CRC.equals("_"+splitText[3])){CRC = "_FAILED";}}
        catch (Exception d) {System.out.print(" EXCEPTION[d]: ");CRC = "_ERROR";}

        if(splitVoltage == 0){
            battery.setVisibility(View.INVISIBLE);
        }else{battery.setVisibility(View.VISIBLE);}

        int batteryPercent = (int)(splitVoltage*156.25 - 556.25);
        battery.setProgress(batteryPercent);


        System.out.print(" incoming:"); System.out.println(incoming);
        System.out.print(" splits:  "); System.out.print(splitAlti);
        System.out.print("   "); System.out.print(splitVelo);
        System.out.print("   "); System.out.print(splitVoltage);
        System.out.print("   CRC:"); System.out.println(CRC);
        System.out.println(" ");

        //long currentMillis = SystemClock.currentThreadTimeMillis();

        long roundedAlti = Math.round(splitAlti);
        TextView splitAltitude = (TextView) findViewById(R.id.splitAltitude);
        splitAltitude.setText(String.valueOf(roundedAlti));

        TextView borderAltitude = (TextView) findViewById(R.id.border);
        borderAltitude.setText(String.valueOf(roundedAlti));

        TextView measureParent = (TextView) findViewById(R.id.measureparent);
        int parentHeight = measureParent.getHeight();
        int ownHeight = splitAltitude.getHeight();

        TextView altitop = (TextView) findViewById(R.id.topAltitude);
        altitop.setText(String.valueOf(Math.round(roundedAlti+0.5)));
        altitop.setTranslationY((float) ((ownHeight+parentHeight/2)*(splitAlti - (int)splitAlti))-(ownHeight+parentHeight/2) );

        TextView altibottom = (TextView) findViewById(R.id.bottomAltitude);
        altibottom.setText(String.valueOf(Math.round(roundedAlti-0.5)));
        altibottom.setTranslationY((float) ((ownHeight+parentHeight/2)*(splitAlti - (int)splitAlti)) );

        float transY = (float) ((ownHeight+parentHeight/2)*(splitAlti - (int)splitAlti));

        /*System.out.print(" parentHeight:");System.out.print(parentHeight);
        System.out.print(" ownHeight:");System.out.print(ownHeight);
        System.out.print(" splitAlti:");System.out.print(splitAlti);
        System.out.print(" transY:");System.out.println(transY);*/

        //int velo = (int) (((splitAlti-prevSplitAlti))*(splitSamples));
        int velo = splitVelo;


        //System.out.print(" incoming:");System.out.print(incoming);
        //System.out.print(" diffAlti:");System.out.print(splitAlti-prevSplitAlti);
        //System.out.print(" currentMillis:");System.out.print(currentMillis);
        //System.out.print(" velo:");System.out.println(velo);

        prevSplitAlti = splitAlti;
        //previousMillis = currentMillis;
        prevSplitVoltage = splitVoltage;
        prevSplitVelo = splitVelo;

        TextView velocity = (TextView) findViewById(R.id.velocity);
        velocity.setText(String.valueOf(velo));

        Paint black = new Paint();
        black.setColor(Color.parseColor("#000000"));

        Paint white = new Paint();
        white.setColor(Color.parseColor("#ffffff"));

        Bitmap bg = Bitmap.createBitmap(64,24, Bitmap.Config.ARGB_8888);

        Canvas canvas = new Canvas(bg);
        canvas.drawRect(0,0,64,24,black);

        /*for(int i = 0; i < 64; i+=5){
            //canvas.drawRect(10,10,11,11,white);
            canvas.drawPoint(i,23,white);
        }*/
        int p = -1*(velo) + 12;
        for(int i = 0; i < 63; i++){
            y[i] = y[i+1];        // Shift all pixels to the left one
            canvas.drawPoint(i, y[i],white);  // Draw all the new pixels except the most recent
        }
        y[63] = p;
        if(y[63]>23){y[63]=23;}
        else if(y[63]<0){y[63]=0;}

        canvas.drawPoint(63, y[63],white);  // Draw the most recent pixel
        for(int i = 0; i < 60; i+=4){
            canvas.drawPoint(i, 12, white);
        }

        FrameLayout ll = (FrameLayout) findViewById(R.id.chart);
        ll.setBackground(new BitmapDrawable(bg));
    }

    /*void liveChart(int v){
        int p = -2*(v) + 24;
        for(int i = 0; i < 63; i++){
            y[i] = y[i+1];        // Shift all pixels to the left one
            canvas.drawPoint(i, y[i]);  // Draw all the new pixels except the most recent
        }
        y[63] = p;
        if(y[63]>47){y[63]=47;}
        else if(y[63]<0){y[63]=0;}

        canvas.drawPoint(63, y[63]);  // Draw the most recent pixel
        for(int i = 0; i < 60; i+=4){
            canvas.drawPoint(i, 24);
        }
    }*/

    public void resetSettingsToDefaults(){
        CheckBox a1000 = ( CheckBox ) findViewById(R.id.aone);
        CheckBox a500 = ( CheckBox ) findViewById(R.id.ahalf);
        CheckBox a250 = ( CheckBox ) findViewById(R.id.aquarter);
        CheckBox a100 = ( CheckBox ) findViewById(R.id.atenth);
        a1000.setChecked(true);
        a500.setChecked(false);
        a250.setChecked(false);
        a100.setChecked(false);
        vSpeedVarioSendData("a1000", false);    // 1000ms averaging

        CheckBox battery = ( CheckBox ) findViewById(R.id.battery);
        battery.setChecked(true);
        batterychecked = true;
        vSpeedVarioSendData("V", false);        // Transmit battery voltage

        CheckBox beep = ( CheckBox ) findViewById(R.id.beep);
        beepchecked = true;
        beep.setChecked(true);
        vSpeedVarioSendData("B", false);        // External Vario Beep Enabled

        CheckBox display = ( CheckBox ) findViewById(R.id.display);
        displaychecked = true;
        display.setChecked(true);
        vSpeedVarioSendData("D", false);        // External Vario Display Enabled
    }

    public void a1000click(View view){
        CheckBox a1000 = ( CheckBox ) findViewById(R.id.aone);
        CheckBox a500 = ( CheckBox ) findViewById(R.id.ahalf);
        CheckBox a250 = ( CheckBox ) findViewById(R.id.aquarter);
        CheckBox a100 = ( CheckBox ) findViewById(R.id.atenth);
        a1000.setChecked(true);
        a500.setChecked(false);
        a250.setChecked(false);
        a100.setChecked(false);
        vSpeedVarioSendData("a1000", false);
    }

    public void a500click(View view){
        CheckBox a1000 = ( CheckBox ) findViewById(R.id.aone);
        CheckBox a500 = ( CheckBox ) findViewById(R.id.ahalf);
        CheckBox a250 = ( CheckBox ) findViewById(R.id.aquarter);
        CheckBox a100 = ( CheckBox ) findViewById(R.id.atenth);
        a1000.setChecked(false);
        a500.setChecked(true);
        a250.setChecked(false);
        a100.setChecked(false);
        vSpeedVarioSendData("a500", false);
    }

    public void a250click(View view){
        CheckBox a1000 = ( CheckBox ) findViewById(R.id.aone);
        CheckBox a500 = ( CheckBox ) findViewById(R.id.ahalf);
        CheckBox a250 = ( CheckBox ) findViewById(R.id.aquarter);
        CheckBox a100 = ( CheckBox ) findViewById(R.id.atenth);
        a1000.setChecked(false);
        a500.setChecked(false);
        a250.setChecked(true);
        a100.setChecked(false);
        vSpeedVarioSendData("a250", false);
    }

    public void a100click(View view){
        CheckBox a1000 = ( CheckBox ) findViewById(R.id.aone);
        CheckBox a500 = ( CheckBox ) findViewById(R.id.ahalf);
        CheckBox a250 = ( CheckBox ) findViewById(R.id.aquarter);
        CheckBox a100 = ( CheckBox ) findViewById(R.id.atenth);
        a1000.setChecked(false);
        a500.setChecked(false);
        a250.setChecked(false);
        a100.setChecked(true);
        vSpeedVarioSendData("a100", false);
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

                        System.out.println(formattedData);

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
