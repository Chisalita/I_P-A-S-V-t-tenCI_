package continental.ingeniously.com.ingeniously.IO.Protocol;

import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;
import android.widget.ArrayAdapter;
import android.widget.Toast;

import java.util.Set;

import continental.ingeniously.com.ingeniously.IO.ConnectThread;
import continental.ingeniously.com.ingeniously.Main.ApplicationBrodcastReciver;
import continental.ingeniously.com.ingeniously.Main.Codes;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public class BluetoothIO implements Protocol, ProtocolObservable {

    private Context mainContext;
    private ConnectThread connection;
  //  private final Handler mainHandler;
    private BluetoothAdapter mBluetoothAdapter;
    private ApplicationBrodcastReciver mReciver;
    private ArrayAdapter<String> BluetoothDevicesArrayAdapter;
    private boolean isRegistered = false;
    private ProtocolResponse currentResponse = new ProtocolResponse();

    private boolean isResponseStarted = false;


    private final Handler mainHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case Codes.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    decodeResponseData(readBuf, msg.arg1);

                    /*
                    // construct a string from the valid bytes in the buffer
                    String readMessage = new String(readBuf, 0, msg.arg1);
                    System.out.println("Recived: " + readMessage);

                    for(int i= 0; i< msg.arg1; i++){
                        System.out.print(String.format("%02X ", readBuf[i]));
                    }

                    System.out.println("---------------------------");
                    //ShowToastMesage("Recived: " + readMessage);
                    */

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

    private void decodeResponseData(byte[] data, int length) {
            if(!isResponseStarted){ //if the response starts now
                isResponseStarted = true;

                switch (length){
                    default:
                    case 2:
                        currentResponse.setNo_of_sensors(data[1]);
                    case 1:
                        currentResponse.setHeader(data[0]);
                        break;
                }

                byte no_of_Sens = currentResponse.getNo_of_sensors();

                if(length > 2){

                    if(length >= no_of_Sens+2){ // iterate all the sensor info
                        for(int i = 0; i<no_of_Sens; i++){
                            currentResponse.setInfo(data[2+i],i);
                        }
                    }else{// iterate just as much that we have bow
                        for(int i = 0; i<length-2; i++){
                            currentResponse.setInfo(data[2+i],i);
                        }
                    }

                    if(length == no_of_Sens+3){
                        currentResponse.setTime_High(data[no_of_Sens+2]);
                    }else if(length > no_of_Sens+3){
                        short t =0;
                        t |= (data[no_of_Sens+2] << 8);
                        t |= data[no_of_Sens+3];
                        currentResponse.setTime(t);
                    }

                    if(length == no_of_Sens+4){
                        currentResponse.setCRC_High(data[no_of_Sens+4]);
                    }else if(length > no_of_Sens+4){
                        short c =0;
                        c |= (data[no_of_Sens+4] << 8);
                        c |= data[no_of_Sens+5];
                        currentResponse.setCRC(c);

                        // the response is over
                        if(currentResponse.verifyCRC()){
                            notifyResponseArrived(currentResponse);
                        }
                        isResponseStarted = false;
                        currentResponse = new ProtocolResponse(); //CRC is not valid is also discards the response
                    }

                }

            }else{ // it is in the middle of the response


            }


    }

    public BluetoothIO(Context context, BluetoothAdapter ba){//, Handler handler){
        //mainHandler = handler;
        mBluetoothAdapter  = ba;
        mReciver = new ApplicationBrodcastReciver(BluetoothDevicesArrayAdapter);
        mainContext = context;
    }


    @Override
    public ProtocolResponse getLastResponse() {
        return null;
    }

    @Override
    public ProtocolCommand getLastCommand() {
        return null;
    }

    @Override
    public void sendCommand(ProtocolCommand command) {

        if (connection != null && command != null) {
            System.out.println("Sends");

            byte cm[]=command.getBytes();

            System.out.print("Command: ");
            for(int i=0; i<cm.length; i++){
                System.out.print(cm[i]+";" );
            }
            System.out.print("\n");
            //command.setForward((byte)40);
            System.out.println("Verify: "+command.verifyCRC());

            connection.sendData(command.getBytes());
        }

    }

    @Override
    public void connect() {

        BluetoothDevice carBluetooth = getPairedDeviceByName(Codes.CAR_BLUETOOTH_NAME);
        if (carBluetooth == null) {
            System.out.println("Bluetooth not found in paired devices");
           // Toast.makeText(this, Codes.CAR_BLUETOOTH_NAME + " Bluetooth not found in paired devices", Toast.LENGTH_LONG).show();
        } else {

            if (connection != null) {
                connection.cancel();
            }
            connection = new ConnectThread(carBluetooth, mainHandler);
            connection.start();
        }


    }

    @Override
    public void disconnect() {
        if (connection != null) {
            connection.cancel();
        }

    }

    @Override
    public void clearAll() {

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


    public void unRegisterBluetoothSearch(){
        if (isRegistered) {
            mainContext.unregisterReceiver(mReciver);
            isRegistered = false;
        }

    }

    private void SearchForBluetoothDevices() {
        // Searches for bluetooth devices


        // Register the BroadcastReceiver
        if (!isRegistered) {
            IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_FOUND);
            mainContext.registerReceiver(mReciver, filter); // Don't forget to unregister during onDestroy
            isRegistered = true;
        }

        mBluetoothAdapter.cancelDiscovery();
        mBluetoothAdapter.startDiscovery();

    }

    private void ShowToastMesage(String s) {
        Toast.makeText(mainContext, s, Toast.LENGTH_SHORT).show();

    }

    @Override
    public void notifyResponseArrived(ProtocolResponse response) {
        ProtocolResponse resp = new ProtocolResponse(response);
        /// notify...


    }
}
