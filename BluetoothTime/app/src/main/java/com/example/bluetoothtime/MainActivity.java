package com.example.bluetoothtime;

import android.Manifest;
import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Context;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Set;
import java.util.UUID;

public class MainActivity extends AppCompatActivity {

    private static final String TAG = "BluetoothTime";
    private static final UUID MY_UUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    private static final int REQUEST_ENABLE_BT = 1;
    private static final int REQUEST_PERMISSIONS_CODE = 2;

    private static final int BT_STATE_CONNECTED = 1;
    private static final int BT_STATE_CONNECTION_FAILED = 2;

    private BluetoothAdapter BTAdaptor;
    private Set<BluetoothDevice> BTPairedDevices;
    private BluetoothDevice BTDevice;
    private BluetoothSocket BTSocket;
    private boolean bBTConnected = false;
    private cBluetoothConnect cBTConnect;
    private classBTInitDataCommunication cBTInitSendReceive;

    private Spinner spinnerBTPairedDevices;
    private Button buttonBTConnect;
    private Button buttonSendTime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Initialize UI elements
        spinnerBTPairedDevices = findViewById(R.id.spinnerBTPairedDevices);
        buttonBTConnect = findViewById(R.id.buttonBTConnect);
        buttonSendTime = findViewById(R.id.buttonSendTime);

        // Initialize Bluetooth
        initializeBluetooth();

        // Set up listeners
        buttonBTConnect.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                handleBluetoothConnection();
            }
        });

        buttonSendTime.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                sendTime();
            }
        });
    }

    private void initializeBluetooth() {
        BTAdaptor = BluetoothAdapter.getDefaultAdapter();
        if (BTAdaptor == null) {
            Toast.makeText(this, "Bluetooth is not supported on this device", Toast.LENGTH_SHORT).show();
            return;
        }
        if (!BTAdaptor.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, REQUEST_ENABLE_BT);
        } else {
            getBTPairedDevices();
        }
    }

    private void getBTPairedDevices() {
        if (BTAdaptor != null) {
            BTPairedDevices = BTAdaptor.getBondedDevices();
            if (BTPairedDevices.size() > 0) {
                ArrayList<String> alPairedDevices = new ArrayList<>();
                for (BluetoothDevice device : BTPairedDevices) {
                    alPairedDevices.add(device.getName() + "\n" + device.getAddress());
                }
                populateSpinnerWithBTPairedDevices(alPairedDevices);
            } else {
                Toast.makeText(this, "No paired Bluetooth devices found", Toast.LENGTH_SHORT).show();
            }
        } else {
            Toast.makeText(this, "Bluetooth Adapter is null", Toast.LENGTH_SHORT).show();
        }
    }

    private void populateSpinnerWithBTPairedDevices(ArrayList<String> alPairedDevices) {
        ArrayAdapter<String> aaPairedDevices = new ArrayAdapter<>(this, android.R.layout.simple_spinner_item, alPairedDevices);
        aaPairedDevices.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        spinnerBTPairedDevices.setAdapter(aaPairedDevices);
    }

    private void handleBluetoothConnection() {
        if (!bBTConnected) {
            if (spinnerBTPairedDevices.getSelectedItemPosition() == 0) {
                Toast.makeText(this, "Please select a Bluetooth Device", Toast.LENGTH_SHORT).show();
                return;
            }
            String sSelectedDevice = spinnerBTPairedDevices.getSelectedItem().toString();
            for (BluetoothDevice device : BTPairedDevices) {
                if (sSelectedDevice.equals(device.getName() + "\n" + device.getAddress())) {
                    BTDevice = device;
                    cBTConnect = new cBluetoothConnect(this, BTDevice);
                    cBTConnect.start();
                }
            }
        } else {
            if (BTSocket != null && BTSocket.isConnected()) {
                try {
                    BTSocket.close();
                    buttonBTConnect.setText("Connect");
                    bBTConnected = false;
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private void sendTime() {
        // Get current time
        Calendar calendar = Calendar.getInstance();
        int hours = calendar.get(Calendar.HOUR_OF_DAY);
        int minutes = calendar.get(Calendar.MINUTE);

        // Format the time as "H24M25;"
        String timeMessage = String.format("H%02dM%02d;", hours, minutes);

        if (BTSocket != null && bBTConnected) {
            try {
                cBTInitSendReceive.write(timeMessage.getBytes());
                Toast.makeText(this, "Time sent: " + timeMessage, Toast.LENGTH_SHORT).show();
            } catch (Exception exp) {
                exp.printStackTrace();
            }
        } else {
            Toast.makeText(this, "Please connect to Bluetooth", Toast.LENGTH_SHORT).show();
        }
    }

    private class cBluetoothConnect extends Thread {
        private BluetoothDevice device;
        private Context context;

        public cBluetoothConnect(Context context, BluetoothDevice BTDevice) {
            this.context = context;
            this.device = BTDevice;
            try {
                if (ActivityCompat.checkSelfPermission(context, Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
                    ActivityCompat.requestPermissions((Activity) context, new String[]{Manifest.permission.BLUETOOTH_CONNECT}, REQUEST_PERMISSIONS_CODE);
                    return;
                }
                BTSocket = device.createRfcommSocketToServiceRecord(MY_UUID);
            } catch (Exception exp) {
                Log.e(TAG, "classBTConnect-exp" + exp.getMessage());
            }
        }

        @Override
        public void run() {
            try {
                BTSocket.connect();
                Message message = Message.obtain();
                message.what = BT_STATE_CONNECTED;
                handler.sendMessage(message);
            } catch (IOException e) {
                e.printStackTrace();
                Message message = Message.obtain();
                message.what = BT_STATE_CONNECTION_FAILED;
                handler.sendMessage(message);
            }
        }
    }

    private class classBTInitDataCommunication extends Thread {
        private final BluetoothSocket bluetoothSocket;
        private InputStream inputStream = null;
        private OutputStream outputStream = null;

        public classBTInitDataCommunication(BluetoothSocket socket) {
            bluetoothSocket = socket;
            try {
                inputStream = bluetoothSocket.getInputStream();
                outputStream = bluetoothSocket.getOutputStream();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }

        public void run() {
            byte[] buffer = new byte[1024];
            int bytes;

            while (BTSocket.isConnected()) {
                try {
                    bytes = inputStream.read(buffer);
                    // No action needed for received messages now
                } catch (IOException e) {
                    e.printStackTrace();
                    Log.e(TAG, "BT disconnect from device end, exp " + e.getMessage());
                    try {
                        if (BTSocket != null && BTSocket.isConnected()) {
                            BTSocket.close();
                        }
                        buttonBTConnect.setText("Connect");
                        bBTConnected = false;
                    } catch (IOException ex) {
                        ex.printStackTrace();
                    }
                }
            }
        }

        public void write(byte[] bytes) {
            try {
                outputStream.write(bytes);
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }

    private final Handler handler = new Handler(new Handler.Callback() {
        @Override
        public boolean handleMessage(Message msg) {
            switch (msg.what) {
                case BT_STATE_CONNECTED:
                    buttonBTConnect.setText("Disconnect");
                    bBTConnected = true;
                    cBTInitSendReceive = new classBTInitDataCommunication(BTSocket);
                    cBTInitSendReceive.start();
                    break;
                case BT_STATE_CONNECTION_FAILED:
                    buttonBTConnect.setText("Connect");
                    bBTConnected = false;
                    Toast.makeText(MainActivity.this, "Connection Failed", Toast.LENGTH_SHORT).show();
                    break;
            }
            return true;
        }
    });
}
