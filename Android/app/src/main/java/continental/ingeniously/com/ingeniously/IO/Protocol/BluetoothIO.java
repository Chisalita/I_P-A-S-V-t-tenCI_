package continental.ingeniously.com.ingeniously.IO.Protocol;

import android.app.Activity;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Message;
import android.widget.ArrayAdapter;
import android.widget.Toast;

import java.util.Arrays;
import java.util.Observer;
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
    private boolean connectedToCar = false;
    private ProtocolResponse currentResponse = new ProtocolResponse();

    //this stores the last response only if it is valid!
    private ProtocolResponse lastResponse = new ProtocolResponse();
    //private Class<? extends ProtocolObserver> observer;
    private ProtocolObserver observer;

    private boolean isResponseStarted = false;


    private final Handler mainHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {

            switch (msg.what) {
                case Codes.MESSAGE_READ:
                    byte[] readBuf = (byte[]) msg.obj;
                    //decodeResponseData(readBuf, msg.arg1);
                    decodeResponseData2(readBuf);
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
                    connectedToCar = true;
                    System.out.println("Connection established");
                    ShowToastMesage("Connection established");
                    break;
                case Codes.CONNECTION_LOST:
                    connectedToCar = false;
                    ShowToastMesage("Connection lost!!");
                    //Connection lost, so try to reconnect
                    //tryConnection();
                    break;
            }

        }
    };

    private void decodeResponseData2(byte[] data) {

      /*  for(int i=0; i< data.length; i++){
            System.out.println("Am primit: "+String.format(" %02X", data[i]));
        }
*/
        currentResponse = new ProtocolResponse();
        currentResponse.setHeader(data[0]);
        currentResponse.setNo_of_sensors(data[1]);
        currentResponse.setInfo(Arrays.copyOfRange(data, 2, 2 + currentResponse.getNo_of_sensors())); //+1
        currentResponse.setTime_High(data[8]);
        currentResponse.setTime_Low(data[9]);
        currentResponse.setCRC_High(data[10]);
        currentResponse.setCRC_Low(data[11]);

        if(currentResponse.verifyCRC()){
            notifyResponseArrived(currentResponse);
        }

        discardCurrentResponse();

    }


    private void decodeResponseData(byte[] data, int length) {

        for(int i=0; i< length; i++){
            System.out.println("Am primit: "+String.format(" %02X", data[i]));
        }


            if(!isResponseStarted){ //if the response starts now
                System.out.println("Intr-una");

                isResponseStarted = true;

                switch (length){
                    default:
                    case 2:
                        if(data[1]>=0){
                            currentResponse.setNo_of_sensors(data[1]);
                        }else{
                            //discard
                            System.out.println("Discarded");
                            currentResponse = new ProtocolResponse();
                            isResponseStarted = false;
                            return;
                        }
                    case 1:
                        currentResponse.setHeader(data[0]);
                        break;
                }

                byte no_of_Sens = currentResponse.getNo_of_sensors();

                if(length > 2){

                    if(length >= no_of_Sens+2){ // iterate all the sensor info
                        for(int i = 0; i<no_of_Sens; i++){
                            try {
                                currentResponse.setInfo(data[2+i],i);
                            }catch (Exception e){
                                e.printStackTrace();
                                discardCurrentResponse();
                                return;
                            }
                        }
                    }else{// iterate just as much that we have now
                        for(int i = 0; i<length-2; i++){
                            try {
                                currentResponse.setInfo(data[2+i],i);
                            } catch (Exception e) {
                                e.printStackTrace();
                                discardCurrentResponse();
                                return;
                            }
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
                        c |= data[(no_of_Sens+5) & 0xff];
                        currentResponse.setCRC(c);

                        // the response is over
                        if(currentResponse.verifyCRC()){
                            notifyResponseArrived(currentResponse);
                        }
                        discardCurrentResponse();
                        //isResponseStarted = false;
                        //currentResponse = new ProtocolResponse(); //if CRC is not valid this also discards the response
                    }

                }

            }else{ // it is in the middle of the response

                System.out.println("separat");
                for(int i=0; i<length; i++){//for all data received check where to put in response

                    int index_pos = currentResponse.getIndex();
                    switch (index_pos){
                        case 0:
                            currentResponse.setHeader(data[i]);
                        case 1:
                            if(data[i]>=0){
                                currentResponse.setNo_of_sensors(data[i]);
                            }else{
                                //discard
                                discardCurrentResponse();
                                //currentResponse = new ProtocolResponse();
                                //isResponseStarted = false;
                            }
                            break;
                        case 2:
                            try {
                                currentResponse.setInfo(data[i],0);
                            } catch (Exception e) {
                                e.printStackTrace();
                                discardCurrentResponse();
                                return;
                            }
                            break;
                        default:
                            byte no_of_sens = currentResponse.getNo_of_sensors();

                            if(index_pos < (no_of_sens + 2)){ //continue setting info
                                System.out.println("index_pos < (no_of_sens + 3)\t"+index_pos+"\t< "+no_of_sens);
                                try {
                                    currentResponse.setInfo(data[i], index_pos-2);
                                } catch (Exception e) {
                                    e.printStackTrace();
                                    discardCurrentResponse();
                                }

                            }else {//finished setting the info
                                if(index_pos == (no_of_sens+2)){
                                    currentResponse.setTime_High(data[i]);
                                }else if(index_pos == (no_of_sens+3)){
                                    currentResponse.setTime_Low(data[i]);
                                }else if(index_pos == (no_of_sens+4)){
                                    currentResponse.setCRC_High(data[i]);
                                    System.out.println("CRC_high: "+String.format(" %02X", data[i]));
                                }else if(index_pos == (no_of_sens+5)){
                                    System.out.println("CRC_low: "+String.format(" %02X", data[i]));
                                    currentResponse.setCRC_Low(data[i]);
                                    // the response is complete
                                    System.out.println("crc"+currentResponse.getCRC()+"CRC corect>: "+currentResponse.verifyCRC());
                                    if(currentResponse.verifyCRC()){
                                        notifyResponseArrived(currentResponse);
                                        System.out.println("gata  e bun");
                                    }
                                    System.out.println("discarded");
                                    discardCurrentResponse();
                                    //isResponseStarted = false;
                                    //currentResponse = new ProtocolResponse(); //if CRC is not valid this also discards the response

                                }else { //something went wrong
                                    System.out.println("something went wrong 2");
                                    discardCurrentResponse();
                                    return;
                                    //currentResponse = new ProtocolResponse();//discard the current response
                                    //isResponseStarted = false;
                                }

                            }

                    }

                }

            }


    }

    public  BluetoothIO(Context context, ProtocolObserver theClass){//, BluetoothAdapter ba){//, Handler handler){
        //mainHandler = handler;
        mBluetoothAdapter  = BluetoothAdapter.getDefaultAdapter();
        if (mBluetoothAdapter == null) {
            // Device does not support Bluetooth
            ShowToastMesage( "There is no Bluetooth on this device!");
            //finish();
        }
        mReciver = new ApplicationBrodcastReciver(BluetoothDevicesArrayAdapter);
        mainContext = context;
        observer = theClass;
        TurnBluetoothON();
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
    public void forceResponseArrived_DEBUG(ProtocolResponse response) {
        notifyResponseArrived(response);
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

    public boolean isConnected(){
        return connectedToCar;
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

    private void discardCurrentResponse(){
        currentResponse = new ProtocolResponse();
        isResponseStarted = false;
    }

    private void ShowToastMesage(String s) {
        Toast.makeText(mainContext, s, Toast.LENGTH_SHORT).show();

    }

    private void TurnBluetoothON(){
        // Turns the bluetooth on if it is not yet on
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableBluetooth = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            ((Activity) mainContext).startActivityForResult(enableBluetooth, Codes.REQUEST_ENABLE_BT);
        } else {
            ShowToastMesage("Bluetooth is already on");
        }
    }

    private void TurnBluetoothOff() {
        // Stops the bluetooth on the device
        if (mBluetoothAdapter.isEnabled()) {
            mBluetoothAdapter.disable();
        }

    }

    @Override
    public void notifyResponseArrived(ProtocolResponse response) {
        System.out.println("BluetoothIO: response arrived!");
        lastResponse = new ProtocolResponse(response);
        /// notify...
        observer.responseArrived(lastResponse);
        //observer.cast(observer).responseArrived(lastResponse);
    }
}
