package continental.ingeniously.com.ingeniously.Main;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.content.IntentFilter;
import android.graphics.Color;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.support.v7.app.ActionBarActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.WindowManager;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Toast;
import android.widget.CheckBox;

import java.util.HashSet;
import java.util.Set;

import continental.ingeniously.com.ingeniously.IO.AcceptThread;
import continental.ingeniously.com.ingeniously.IO.ConnectThread;
import continental.ingeniously.com.ingeniously.IO.Protocol.BluetoothIO;
import continental.ingeniously.com.ingeniously.IO.Protocol.Protocol;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolCommand;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolObservable;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolObserver;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;
import continental.ingeniously.com.ingeniously.Processing.Logic;
import continental.ingeniously.com.ingeniously.R;


public class Main extends ActionBarActivity implements ProtocolObserver,View.OnClickListener {

   // Comment
   // private BluetoothAdapter mBluetoothAdapter;
    private ArrayAdapter<String> BluetoothDevicesArrayAdapter;
    //private ConnectThread connection;
    private ApplicationBrodcastReciver mReciver;
    private boolean isRegisterd = false;
    private BluetoothIO bluetoothIO;
    private Button exitA;
    private Button exitB;
    private Button exitC;
    private Button exitD;
    private Button position1;
    private Button position2;
    private Button position3;
    private Button position4;
    private Button position5;
    private Button position6;
    private CheckBox sidewaysBox;
    private int exit;
    private int parkPosition;
    private Logic logic;

/*
    private final Handler MainHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case Codes.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    System.out.print("Recived: ");
                   // System.out.println("Recived: " + readMessage);
                    for(int i=0; i<msg.arg1; i++){
                        //if(readBuf[i]<'0'){
                         //   System.out.print((char)(readBuf[i]+'0'));
                        //}else{
                            System.out.print((char)(readBuf[i]));
                        //}
                    }
                    System.out.println("-----------------------------------------------------------");

                    //ShowToastMesage("Recived: " + readMessage);
                    break;
                case Codes.CONNECTION_ESTABLISHED:
                    System.out.println("Connection established");
                    ShowToastMesage("Connection established");
                    break;
                case Codes.CONNECTION_LOST:
                    ShowToastMesage("Connection lost!!");
                    //Connection lost, so try to reconnect
                    //tryConnection();
                    break;
            }

        }
    };
*/

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        //setContentView(R.layout.activity_main);
        setContentView(R.layout.activity_starter_);

        // Do not let screen to be turned off
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);


       /* mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            // Device does not support Bluetooth
            Toast.makeText(this, "There is no Bluetooth on this device!", Toast.LENGTH_LONG).show();
            //finish();
        }*/



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


        //TurnBluetoothOn();
        mReciver = new ApplicationBrodcastReciver(BluetoothDevicesArrayAdapter);
        //printPairedDevices();

        //tryConnection();
        bluetoothIO = new BluetoothIO(this, this);//,mBluetoothAdapter);//, MainHandler);
        bluetoothIO.connect();


        exitA = (Button) findViewById(R.id.exitA);
        exitA.setBackgroundResource(android.R.drawable.btn_default);
        exitB = (Button) findViewById(R.id.exitB);
        exitB.setBackgroundResource(android.R.drawable.btn_default);
        exitC = (Button) findViewById(R.id.exitC);
        exitC.setBackgroundResource(android.R.drawable.btn_default);
        exitD = (Button) findViewById(R.id.exitD);
        exitD.setBackgroundResource(android.R.drawable.btn_default);
        position1 = (Button) findViewById(R.id.position1);
        position1.setBackgroundResource(android.R.drawable.btn_default);
        position2 = (Button) findViewById(R.id.position2);
        position2.setBackgroundResource(android.R.drawable.btn_default);
        position3 = (Button) findViewById(R.id.position3);
        position3.setBackgroundResource(android.R.drawable.btn_default);
        position4 = (Button) findViewById(R.id.position4);
        position4.setBackgroundResource(android.R.drawable.btn_default);
        position5 = (Button) findViewById(R.id.position5);
        position5.setBackgroundResource(android.R.drawable.btn_default);
        position6 = (Button) findViewById(R.id.position6);
        position6.setBackgroundResource(android.R.drawable.btn_default);
        sidewaysBox = (CheckBox) findViewById(R.id.sidewaysBox);

        exitA.setOnClickListener(this);
        exitB.setOnClickListener(this);
        exitC.setOnClickListener(this);
        exitD.setOnClickListener(this);
        position1.setOnClickListener(this);
        position2.setOnClickListener(this);
        position3.setOnClickListener(this);
        position4.setOnClickListener(this);
        position5.setOnClickListener(this);
        position6.setOnClickListener(this);
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
            bluetoothIO.disconnect();

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

        if (bluetoothIO != null) {
            bluetoothIO.unRegisterBluetoothSearch();
        }


        /*if (isRegisterd) {
            unregisterReceiver(mReciver);
            isRegisterd = false;
        }
        */

        /*
        //Nu stiu daca este corect sau daca nu cumva ar trebui sa apelez cancel dintr-o metoda diferita
        if(connection!=null) {
            connection.cancel();
        }*/


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

 /*   public void SearchForBluetoothDevices() {
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
/*
    public void TurnBluetoothOn() {
        // Turns the bluetooth on if it is not yet on
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBluetooth, Codes.REQUEST_ENABLE_BT);
        } else {
            Toast.makeText(this, "Bluetooth is already on", Toast.LENGTH_SHORT).show();
        }

    }

*/
 /*   public void TurnBluetoothOff(View v) {
        // Stops the bluetooth on the device
        if (mBluetoothAdapter.isEnabled()) {
            mBluetoothAdapter.disable();
        }

    }
*/
/*    private void tryConnection() {

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
*/

    /*
    public void StartBluetoothServer(View v) {
        // Starts the bluetooth server for accepting connections

        // create a new thread to accept incoming connections
        AcceptThread serverAccepter = new AcceptThread(MainHandler);

        //start accepting connections
        serverAccepter.start();

    }
*/

    public void Start(View v) {
/*
        //System.out.println(ParkiPos_editText.getText());
        //System.out.println(ExitNo_editText.getText());

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


*/
        logic = new Logic(this,this);

        logic.setParkPos(parkPosition);
        logic.setExit(exit);

        if(sidewaysBox.isChecked()) {
            logic.setMode(Codes.MODE_SIDEWAYS);
        }
        else {
            logic.setMode(Codes.MODE_NORMAL);
        }

        ShowToastMesage("Exit: "+exit+", "+"Park position: "+parkPosition);

        //String message = ParkPos.getText().toString();
        String message = "";

        if (message == null || message.isEmpty()) {
            message = "S";
        } else {
            message = message.substring(0, 1);
        }
        ProtocolCommand comm = new ProtocolCommand((byte) 10, (byte) 38, (byte) ((message.charAt(0) - '0') * 10), (short) 2056);

        bluetoothIO.sendCommand(comm);
    }


/*    private Set<BluetoothDevice> getPairedDevices() {
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
*/
/*    //#######################TEST###################################
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
*/

    public void ShowToastMesage(String s) {
        Toast.makeText(this, s, Toast.LENGTH_SHORT).show();
    }

    /*BluetoothDevice getPairedDeviceByName(String name) {
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
*/

    @Override
    public void responseArrived(ProtocolResponse response) {

    }

    public void setExitPoint(String s){
        if(s == "A")
            ShowToastMesage("Exit A");
        if(s == "B")
            ShowToastMesage("Exit B");
        if(s == "C")
            ShowToastMesage("Exit C");
        if(s == "D")
            ShowToastMesage("Exit D");
    }

    public void setParkPosition(String s) {
        if(s == "1")
            ShowToastMesage("Park position 1");
        if(s == "2")
            ShowToastMesage("Park position 2");
        if(s == "3")
            ShowToastMesage("Park position 3");
        if(s == "4")
            ShowToastMesage("Park position 4");
        if(s == "5")
            ShowToastMesage("Park position 5");
        if(s == "6")
            ShowToastMesage("Park position 6");
    }

    @Override
    public void onClick(View v) {
        switch(v.getId()) {
            case R.id.exitA:
                setExitPoint("A");
                exit = 1;
                exitA.setBackgroundColor(Color.GREEN);
                exitB.setBackgroundResource(android.R.drawable.btn_default);
                exitC.setBackgroundResource(android.R.drawable.btn_default);
                exitD.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.exitB:
                setExitPoint("B");
                exit = 2;
                exitA.setBackgroundResource(android.R.drawable.btn_default);
                exitB.setBackgroundColor(Color.GREEN);
                exitC.setBackgroundResource(android.R.drawable.btn_default);
                exitD.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.exitC:
                setExitPoint("C");
                exit = 3;
                exitA.setBackgroundResource(android.R.drawable.btn_default);
                exitB.setBackgroundResource(android.R.drawable.btn_default);
                exitC.setBackgroundColor(Color.GREEN);
                exitD.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.exitD:
                setExitPoint("D");
                exit = 0;
                exitA.setBackgroundResource(android.R.drawable.btn_default);
                exitB.setBackgroundResource(android.R.drawable.btn_default);
                exitC.setBackgroundResource(android.R.drawable.btn_default);
                exitD.setBackgroundColor(Color.GREEN);
                break;
        }
        switch(v.getId()) {
            case R.id.position1:
                setParkPosition("1");
                parkPosition = 1;
                position1.setBackgroundColor(Color.GREEN);
                position2.setBackgroundResource(android.R.drawable.btn_default);
                position3.setBackgroundResource(android.R.drawable.btn_default);
                position4.setBackgroundResource(android.R.drawable.btn_default);
                position5.setBackgroundResource(android.R.drawable.btn_default);
                position6.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.position2:
                setParkPosition("2");
                parkPosition = 2;
                position1.setBackgroundResource(android.R.drawable.btn_default);
                position2.setBackgroundColor(Color.GREEN);
                position3.setBackgroundResource(android.R.drawable.btn_default);
                position4.setBackgroundResource(android.R.drawable.btn_default);
                position5.setBackgroundResource(android.R.drawable.btn_default);
                position6.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.position3:
                setParkPosition("3");
                parkPosition = 3;
                position1.setBackgroundResource(android.R.drawable.btn_default);
                position2.setBackgroundResource(android.R.drawable.btn_default);
                position3.setBackgroundColor(Color.GREEN);
                position4.setBackgroundResource(android.R.drawable.btn_default);
                position5.setBackgroundResource(android.R.drawable.btn_default);
                position6.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.position4:
                setParkPosition("4");
                parkPosition = 4;
                position1.setBackgroundResource(android.R.drawable.btn_default);
                position2.setBackgroundResource(android.R.drawable.btn_default);
                position3.setBackgroundResource(android.R.drawable.btn_default);
                position4.setBackgroundColor(Color.GREEN);
                position5.setBackgroundResource(android.R.drawable.btn_default);
                position6.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.position5:
                setParkPosition("5");
                parkPosition = 5;
                position1.setBackgroundResource(android.R.drawable.btn_default);
                position2.setBackgroundResource(android.R.drawable.btn_default);
                position3.setBackgroundResource(android.R.drawable.btn_default);
                position4.setBackgroundResource(android.R.drawable.btn_default);
                position5.setBackgroundColor(Color.GREEN);
                position6.setBackgroundResource(android.R.drawable.btn_default);
                break;
            case R.id.position6:
                setParkPosition("6");
                parkPosition = 6;
                position1.setBackgroundResource(android.R.drawable.btn_default);
                position2.setBackgroundResource(android.R.drawable.btn_default);
                position3.setBackgroundResource(android.R.drawable.btn_default);
                position4.setBackgroundResource(android.R.drawable.btn_default);
                position5.setBackgroundResource(android.R.drawable.btn_default);
                position6.setBackgroundColor(Color.GREEN);
                break;
        }
    }

    public String toString() {
        return "" + parkPosition;
    }
}
