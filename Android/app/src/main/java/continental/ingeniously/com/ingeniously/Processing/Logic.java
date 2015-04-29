package continental.ingeniously.com.ingeniously.Processing;

import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.widget.Toast;

import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Info;
import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Observable;
import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Observer;
import continental.ingeniously.com.ingeniously.IO.Protocol.BluetoothIO;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolObserver;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/25/2015.
 */
public class Logic implements Processor, ProtocolObserver, HMI_Observable {

    private Context mainContent;
    ProtocolObserver observer;
    private Context mainContext;
    private BluetoothIO bluetoothIO;
    private int ParkPos=0;
    private int Exit=0;
    private int Mode=0;
    private Handler handler = new Handler(){
        @Override
        public void handleMessage(Message msg) {
            ShowToastMesage("Exit: "+ getExit() + ", " + "Park position: " + getParkPos());

        }
    };

    public Logic(Context context, ProtocolObserver protocolObserver){
        observer = protocolObserver;
        mainContext = context;
        bluetoothIO = new BluetoothIO(mainContext, protocolObserver);
        ParkPos =0;
        Exit = 0;
        Mode = 0;
     }

    @Override
    public void StartProcessing() {

        Runnable r = new Runnable() {
            @Override
            public void run() {
                handler.sendEmptyMessage(0);
            }
        };
        Thread startProcessing = new Thread(r);
        startProcessing.start();
    }

    @Override
    public void setParkPos(int parkPos) {
        ParkPos = parkPos;
    }

    @Override
    public void setExit(int exit) {
        Exit = exit;
    }

    @Override
    public void setMode(int mode) {
        Mode = mode;
    }

    @Override
    public void reset() {

    }

    @Override
    public void responseArrived(ProtocolResponse response) {

    }

    @Override
    public void notifyNewInfoToShow(HMI_Info info) {



    }

    public void ShowToastMesage(String s) {
        Toast.makeText(mainContext, s, Toast.LENGTH_SHORT).show();
    }

    public int getExit() {
        return Exit;
    }

    public int getParkPos() {
        return ParkPos;
    }

    public int getMode() {
        return Mode;
    }
}
