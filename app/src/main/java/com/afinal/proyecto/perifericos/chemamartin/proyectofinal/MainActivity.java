package com.afinal.proyecto.perifericos.chemamartin.proyectofinal;


import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.support.constraint.ConstraintLayout;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.inputmethod.InputMethodManager;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ImageButton;
import android.widget.ImageView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.util.UUID;


public class MainActivity extends Activity {
    final String pin_desbloquear = "11111";
    boolean flagLuz = false, flagLocked = true;

    TextView txtString, tempSet, tempActual;
    TextView txtStringLength;
    TextView txtSendorLDR;
    ImageButton botonBombilla;
    ImageButton imageButtonStatusAlarm;
    Handler bluetoothIn;
    String PIN;
    SeekBar seekBarTemp;
    ImageView ldrImageView, flameView;
    EditText pinEditText;
    ImageButton key;
    ConstraintLayout tempLayout;
    FrameLayout framePIN;
    final int handlerState = 0;             //used to identify handler message
    private BluetoothAdapter btAdapter = null;
    private BluetoothSocket btSocket = null;
    private boolean flagNocheBotonControl = false;
    private ConnectedThread mConnectedThread;

    // SPP UUID service - this should work for most devices
    private static final UUID BTMODULEUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");

    @SuppressLint("HandlerLeak")
    @Override
    public void onCreate(final Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        setContentView(R.layout.activity_main);

        //Link the buttons and textViews to respective views

        txtString = findViewById(R.id.txtString);
        txtStringLength =  findViewById(R.id.testView1);
        ldrImageView = findViewById(R.id.imageView2);
        botonBombilla = findViewById(R.id.imageButton2);
        txtSendorLDR = findViewById(R.id.tv_sendorldr);
        imageButtonStatusAlarm = findViewById(R.id.imageView);
        seekBarTemp = findViewById(R.id.seekBar);
        tempSet = findViewById(R.id.tempSetID);
        tempActual = findViewById(R.id.tempActualID);
        pinEditText = findViewById(R.id.editTextNumberPassword);
        key = findViewById(R.id.idKeyButton);
        framePIN = findViewById(R.id.idFramePIN);
        flameView = findViewById(R.id.idFlame);
        tempLayout = findViewById(R.id.constraintLayout);

        seekBarTemp.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
            @Override
            public void onProgressChanged(SeekBar seekBar, int progress, boolean fromUser) {
                tempSet.setText(String.valueOf(seekBar.getProgress() / (float) 10));

            }

            @Override
            public void onStartTrackingTouch(SeekBar seekBar) {

            }

            @Override
            public void onStopTrackingTouch(SeekBar seekBar) {
                mConnectedThread.write("sT" + seekBar.getProgress());
            }
        });
        bluetoothIn = new Handler() {
            @SuppressLint("SetTextI18n")
            public void handleMessage(android.os.Message msg) {
                // if (msg.what == handlerState) {          //if message is what we want

                // msg.arg1 = bytes from connect thread
                //recDataString.append(String.valueOf( msg.what));
                String dataInPrint = (String) msg.obj;    // extract string

                Log.i("recive", dataInPrint);
                //if (dataInPrint.indexOf("~") > 0) {                                           // make sure there data before ~
                txtString.setText("Datos recibidos = " + dataInPrint);
                int dataLength = dataInPrint.length();       //get length of data received
                txtStringLength.setText("Tamaño del String = " + dataLength);
                if (dataInPrint.contains("T")) {
                    Log.i("atindex", String.valueOf(dataInPrint.indexOf("T")));
                    int index = dataInPrint.indexOf("T")-1;
                    try {
                        String temp = String.valueOf(Integer.parseInt(dataInPrint.substring(index + 2, index + 5)) / (float) 10);
                        tempActual.setText(temp);
                    } catch (NumberFormatException e) {
                        e.printStackTrace();
                    }
                }
                if (dataInPrint.contains("D")) {
                    ldrImageView.setImageResource(R.drawable.sun);
                    flagNocheBotonControl=false;
                }
                if (dataInPrint.contains("N")) {
                    ldrImageView.setImageResource(R.drawable.night);

                    if(!flagNocheBotonControl){
                        botonBombilla.setImageResource(R.drawable.bombilla_on);
                        flagLuz = true;
                        mConnectedThread.write("b1");
                    }

                }
                if (dataInPrint.contains("l0"))
                    flameView.setVisibility(View.INVISIBLE);
                if (dataInPrint.contains("l1"))
                    flameView.setVisibility(View.VISIBLE);

                //recDataString.delete(0, recDataString.length());      //clear all string data
                // strIncom =" ";

                //}

            }
        };

        bluetoothIn.removeCallbacksAndMessages(null);

        btAdapter = BluetoothAdapter.getDefaultAdapter();       // get Bluetooth adapter
        checkBTState();


        imageButtonStatusAlarm.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                if (flagLocked) {
                    framePIN.setVisibility(View.VISIBLE);
                    ldrImageView.setVisibility(View.INVISIBLE);
                    flameView.setVisibility(View.INVISIBLE);
                    botonBombilla.setVisibility(View.INVISIBLE);
                    imageButtonStatusAlarm.setVisibility(View.INVISIBLE);
                    seekBarTemp.setVisibility(View.INVISIBLE);
                    tempLayout.setVisibility(View.INVISIBLE);
                    pinEditText.requestFocus();
                    pinEditText.setText(null);
                    final InputMethodManager imm = (InputMethodManager) getSystemService(Service.INPUT_METHOD_SERVICE);
                    assert imm != null;
                    imm.showSoftInput(pinEditText, 0);
                    key.setOnClickListener(new OnClickListener() {
                        @Override
                        public void onClick(View v) {
                            PIN = String.valueOf(pinEditText.getText());
                            framePIN.setVisibility(View.INVISIBLE);
                            imm.toggleSoftInput(InputMethodManager.RESULT_HIDDEN, 0);
                            ldrImageView.setVisibility(View.VISIBLE);

                            botonBombilla.setVisibility(View.VISIBLE);
                            imageButtonStatusAlarm.setVisibility(View.VISIBLE);
                            seekBarTemp.setVisibility(View.VISIBLE);
                            tempLayout.setVisibility(View.VISIBLE);
                            if (PIN != null) {
                                if (PIN.equals(pin_desbloquear)) {
                                    Toast.makeText(getBaseContext(), "PIN Correcto", Toast.LENGTH_SHORT).show();
                                    flagLocked = false;
                                    mConnectedThread.write("A0");
                                    imageButtonStatusAlarm.setImageResource(R.drawable.lock_open);
                                } else {
                                    Toast.makeText(getBaseContext(), "PIN Incorrecto", Toast.LENGTH_SHORT).show();
                                    imageButtonStatusAlarm.setImageResource(R.drawable.lock_close);
                                }
                            }
                        }
                    });

                } else {
                    flagLocked = true;
                    mConnectedThread.write("A1");
                    imageButtonStatusAlarm.setImageResource(R.drawable.lock_close);
                }
            }
        });

        botonBombilla.setOnClickListener(new OnClickListener() {
            @Override
            public void onClick(View v) {
                flagNocheBotonControl= true;
                if (flagLuz) {
                    botonBombilla.setImageResource(R.drawable.bombilla_off);
                    flagLuz = false;
                    mConnectedThread.write("b0");
                } else {
                    botonBombilla.setImageResource(R.drawable.bombilla_on);
                    flagLuz = true;
                    mConnectedThread.write("b1");
                }


            }
        });

    }


    private BluetoothSocket createBluetoothSocket(BluetoothDevice device) throws IOException {

        return device.createRfcommSocketToServiceRecord(BTMODULEUUID);
        //creates secure outgoing connecetion with BT device using UUID
    }

    @Override
    public void onResume() {
        super.onResume();

        //Get MAC address from DeviceListActivity via intent
        Intent intent = getIntent();

        //Get the MAC address from the DeviceListActivty via EXTRA
        // String for MAC address
        String address = intent.getStringExtra(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
        if (address == null)
            address = "00:14:01:03:55:D9";
        //create device and set the MAC address
        Log.i("ramiro", "adress : " + address);
        BluetoothDevice device = btAdapter.getRemoteDevice(address);

        try {
            btSocket = createBluetoothSocket(device);
        } catch (IOException e) {
            Toast.makeText(getBaseContext(), "La creacción del Socket fallo", Toast.LENGTH_LONG).show();
        }
        // Establish the Bluetooth socket connection.
        try {
            btSocket.connect();
        } catch (IOException e) {
            try {
                mConnectedThread.write("y");
                btSocket.close();
            } catch (IOException e2) {
                //insert code to deal with this
            }
        }
        if (mConnectedThread == null) {
            mConnectedThread = new ConnectedThread(btSocket);
            mConnectedThread.start();
        }


        //I send a character when resuming.beginning transmission to check device is connected
        //If it is not an exception will be thrown in the write method and finish() will be called
        mConnectedThread.write("x");
    }

    @Override
    public void onPause() {
        super.onPause();

    }

    //Checks that the Android device Bluetooth is available and prompts to be turned on if off
    private void checkBTState() {

        if (btAdapter == null) {
            Toast.makeText(getBaseContext(), "El dispositivo no soporta bluetooth", Toast.LENGTH_LONG).show();
        } else {
            if (!btAdapter.isEnabled()) {
                Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
                startActivityForResult(enableBtIntent, 1);
            }
        }
    }

    //create new class for connect thread
    private class ConnectedThread extends Thread {
        private final InputStream mmInStream;
        private final OutputStream mmOutStream;

        //creation of the connect thread
        public ConnectedThread(BluetoothSocket socket) {
            InputStream tmpIn = null;
            OutputStream tmpOut = null;

            try {
                //Create I/O streams for connection
                tmpIn = socket.getInputStream();
                tmpOut = socket.getOutputStream();
            } catch (IOException ignored) {
            }

            mmInStream = tmpIn;
            mmOutStream = tmpOut;
        }


        public void run() {
            byte[] buffer = new byte[256];
            int bytes;

            // Keep looping to listen for received messages
            while (true) {
                try {
                    bytes = mmInStream.read(buffer);         //read bytes from input buffer
                    String readMessage = new String(buffer, 0, bytes);
                    // Send the obtained bytes to the UI Activity via handler
                    bluetoothIn.removeCallbacksAndMessages(null);
                    if (readMessage.length() > 1)
                        if (!(bluetoothIn.sendMessage(bluetoothIn.obtainMessage(handlerState, bytes, -1, readMessage))))
                            Log.i("Send", "ERROR en el envio de mensaje");
                } catch (IOException e) {
                    break;
                }
            }
        }

        //write method
        public void write(String input) {
            byte[] msgBuffer = (input + "\0\r\n").getBytes();           //converts entered String into bytes
            try {
                Log.i("write", new String(msgBuffer));
                mmOutStream.flush();
                mmOutStream.write(msgBuffer);                //write bytes over BT connection via outstream
            } catch (IOException e) {
                //if you cannot write, close the application
                Toast.makeText(getBaseContext(), "La Conexión fallo", Toast.LENGTH_LONG).show();
                finish();

            }
        }
    }

    @Override
    public void finish() {
        mConnectedThread.write("y");
        super.finish();

    }
}

