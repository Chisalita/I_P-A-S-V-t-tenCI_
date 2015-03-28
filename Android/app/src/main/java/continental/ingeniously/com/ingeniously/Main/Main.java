package continental.ingeniously.com.ingeniously.Main;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.EditText;
import android.widget.Toast;

import java.util.HashSet;
import java.util.Set;

import continental.ingeniously.com.ingeniously.IO.AcceptThread;
import continental.ingeniously.com.ingeniously.IO.ConnectThread;
import continental.ingeniously.com.ingeniously.R;


public class Main extends ActionBarActivity {

    private BluetoothAdapter mBluetoothAdapter;
    private ArrayAdapter<String> BluetoothDevicesArrayAdapter;
    private ConnectThread connection;
    private ApplicationBrodcastReciver mReciver;
    private boolean isRegisterd = false;
    private EditText ParkPos;
    private EditText ExitNo;


    private final Handler MainHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case Codes.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    System.out.println("Recived: " + readMessage);
                    ShowToastMesage("Recived: " + readMessage);
                    break;
                case Codes.CONNECTION_ESTABLISHED:
                    System.out.println("Connection established");
                    ShowToastMesage("Connection established");
                    break;
                case Codes.CONNECTION_LOST:
                    ShowToastMesage("Connection lost!!");
                    //Connection lost, so try to reconnect
                    tryConnection();
                    break;
            }

        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_starter_);

        // Do not let screen to be turned off
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);


        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            // Device does not support Bluetooth
            Toast.makeText(this, "There is no Bluetooth on this device!", Toast.LENGTH_LONG).show();
            //finish();
        }


        ParkPos = (EditText) findViewById(R.id.ParkingPos_editText);
        ExitNo = (EditText) findViewById(R.id.ExitNo_editText);



        /*
        BluetoothDevicesArrayAdapter = new ArrayAdapter<String>(this, android.R.layout.simple_list_item_1);
        ListView bluetoothDevices_listView = (ListView) findViewById(R.id.bluetoothDevices_listView);
        bluetoothDevices_listView.setAdapter(BluetoothDevicesArrayAdapter);

        // Set what happens when you select an item (a device) from the list
        bluetoothDevices_listView.setOnItemClickListener(new AdapterView.OnItemClickListener() {
            @Override
            public void onItemClick(AdapterView<?> parent, View view, int position, long id) {
                // get the string from the item that was clicked
                String dev = (String) parent.getItemAtPosition(position);
                System.out.println(dev);
            }
        });

       */


        //BluetoothDevicesArrayAdapter.add("Care/");


        // TurnBluetoothOn();
        mReciver = new ApplicationBrodcastReciver(BluetoothDevicesArrayAdapter);
        printPairedDevices();

        tryConnection();


        /*
        BluetoothDevice carBluetooth = getPairedDeviceByName(Codes.CAR_BLUETOOTH_NAME);
        if (carBluetooth == null) {
            Toast.makeText(this, Codes.CAR_BLUETOOTH_NAME + " Bluetooth not found in paired devices", Toast.LENGTH_LONG).show();
        } else {

            if (connection != null) {
                connection.cancel();
            }
            connection = new ConnectThread(carBluetooth, MainHandler);
            connection.start();
        }
        */

       /* Set<BluetoothDevice> paired = getPairedDevices();
        System.out.println("!!!!");

        if(!paired.isEmpty() && paired!=null){
            for(BluetoothDevice p : paired){
                if(p.getName().contains("TEAM10")){
                    if(connection != null){
                        connection.cancel();
                    }
                    connection = new ConnectThread(p,MainHandler);
                    connection.start();
                    System.out.println(p.getName());
                }

            }

        }else {
            System.out.println("NOOOO");
        }
*/
    }


    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        // Inflate the menu; this adds items to the action bar if it is present.
        getMenuInflater().inflate(R.menu.menu_main, menu);
        return true;
    }


    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle action bar item clicks here. The action bar will
        // automatically handle clicks on the Home/Up button, so long
        // as you specify a parent activity in AndroidManifest.xml.
        int id = item.getItemId();

        //noinspection SimplifiableIfStatement
        if (id == R.id.action_settings) {
            ///////TEST/////////
            Intent intent = new Intent(this, Starter_Activity.class);
            startActivity(intent);
            /////////////////
            return true;
        }

        if (id == R.id.action_debug) {
            ///////TEST/////////
            Intent intent = new Intent(this, DebugActivity.class);
            startActivity(intent);
            /////////////////
            return true;
        }

        return super.onOptionsItemSelected(item);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (isRegisterd) {
            unregisterReceiver(mReciver);
            isRegisterd = false;
        }

        //Nu stiu daca este corect sau daca nu cumva ar trebui sa apelez cancel dintr-o metoda diferita
        connection.cancel();

    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        switch (requestCode) {
            case Codes.REQUEST_ENABLE_BT:
                if (resultCode == Activity.RESULT_OK) {
                    // The bluetooth is now enabled

                } else if (resultCode == Activity.RESULT_CANCELED) {
                    // The user declined access to bluetooth
                    Toast.makeText(this, "Make sure to activate Bluetooth!", Toast.LENGTH_LONG).show();
                }

                break;
        }

    }

    /*
    public void SearchForBluetoothDevices(View v) {
        // Searches for bluetooth devices


        // Register the BroadcastReceiver
        if (!isRegisterd) {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            registerReceiver(mReciver, filter); // Don't forget to unregister during onDestroy
            isRegisterd = true;
        }

        mBluetoothAdapter.cancelDiscovery();
        mBluetoothAdapter.startDiscovery();

    }

    */

    public void SearchForBluetoothDevices() {
        // Searches for bluetooth devices


        // Register the BroadcastReceiver
        if (!isRegisterd) {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            registerReceiver(mReciver, filter); // Don't forget to unregister during onDestroy
            isRegisterd = true;
        }

        mBluetoothAdapter.cancelDiscovery();
        mBluetoothAdapter.startDiscovery();

    }

/*
    public void TurnBluetoothOn(View v) {
        // Turns the bluetooth on if it is not yet on
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, Codes.REQUEST_ENABLE_BT);
        } else {
            Toast.makeText(this, "Bluetooth is already on", Toast.LENGTH_SHORT).show();
        }

    }
*/

    public void TurnBluetoothOn() {
        // Turns the bluetooth on if it is not yet on
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, Codes.REQUEST_ENABLE_BT);
        } else {
            Toast.makeText(this, "Bluetooth is already on", Toast.LENGTH_SHORT).show();
        }

    }

    public void TurnBluetoothOff(View v) {
        // Stops the bluetooth on the device
        if (mBluetoothAdapter.isEnabled()) {
            mBluetoothAdapter.disable();
        }

    }

    private void tryConnection() {

        BluetoothDevice carBluetooth = getPairedDeviceByName(Codes.CAR_BLUETOOTH_NAME);
        if (carBluetooth == null) {
            Toast.makeText(this, Codes.CAR_BLUETOOTH_NAME + " Bluetooth not found in paired devices", Toast.LENGTH_LONG).show();
        } else {

            if (connection != null) {
                connection.cancel();
            }
            connection = new ConnectThread(carBluetooth, MainHandler);
            connection.start();
        }

    }

    public void StartBluetoothServer(View v) {
        // Starts the bluetooth server for accepting connections

        // create a new thread to accept incoming connections
        AcceptThread serverAccepter = new AcceptThread(MainHandler);

        //start accepting connections
        serverAccepter.start();

    }

    public void Start(View v) {
        /*
        System.out.println(ParkiPos_editText.getText());
        System.out.println(ExitNo_editText.getText());
        */
        String message = ParkPos.getText().toString();

        if (message == null || message.isEmpty()) {
            message = "S";
        } else {
            message = message.substring(0, 1);
        }


        if (connection != null) {
            System.out.println("Sends");

            connection.sendData(message);
        }
    }

    public void ModeSelect(View v) {
        switch (v.getId()) {
            case R.id.Normal_radioButton:

                break;
            case R.id.Sideways_radioButton:

                break;
        }

    }


    private Set<BluetoothDevice> getPairedDevices() {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();
        Set<BluetoothDevice> mine = new HashSet<BluetoothDevice>();

        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                // Add the name and address to an array adapter to show in a ListView
                if (device.getName().contains("TEAM10")) {
                    mine.add(device);
                    System.out.println("Added " + device.getName());
                }
            }
        }

        return mine;
    }

    //#######################TEST###################################
    private void printPairedDevices() {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();


        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                // Add the name and address to an array adapter to show in a ListView
                System.out.println("Added " + device.getName());
            }
        }

    }
    //#######################TEST###################################


    public void ShowToastMesage(String s) {
        Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
    }

    BluetoothDevice getPairedDeviceByName(String name) {
        Set<BluetoothDevice> pairedDevices = mBluetoothAdapter.getBondedDevices();

        // If there are paired devices
        if (pairedDevices.size() > 0) {
            // Loop through paired devices
            for (BluetoothDevice device : pairedDevices) {
                //Found the desired device
                if (device.getName().contains(name)) {
                    // return the device
                    return device;
                }
            }
        }
        return null;
    }

}
