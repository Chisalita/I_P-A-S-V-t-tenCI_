package continental.ingeniously.com.ingeniously.Processing;

import android.content.Context;

import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Info;
import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Observable;
import continental.ingeniously.com.ingeniously.IO.HMI.HMI_Observer;
import continental.ingeniously.com.ingeniously.IO.Protocol.BluetoothIO;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolCommand;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolObserver;
import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;
import continental.ingeniously.com.ingeniously.Main.Codes;
import continental.ingeniously.com.ingeniously.Processing.States.ExitFound_State;
import continental.ingeniously.com.ingeniously.Processing.States.ExitParkingSpot_State;
import continental.ingeniously.com.ingeniously.Processing.States.Idle_State;
import continental.ingeniously.com.ingeniously.Processing.States.SearchForExit_State;
import continental.ingeniously.com.ingeniously.Processing.States.SearchForParkingSpot_State;
import continental.ingeniously.com.ingeniously.Processing.States.State;
import continental.ingeniously.com.ingeniously.Processing.States.StateObserver;
import continental.ingeniously.com.ingeniously.Processing.States.States;
import continental.ingeniously.com.ingeniously.Processing.States.StayStationary_State;
import continental.ingeniously.com.ingeniously.Processing.States.TryParking_State;

/**
 * Created by chisa_000 on 4/25/2015.
 */
public class Logic implements Processor, ProtocolObserver, HMI_Observable, StateObserver {
    State currentState = new Idle_State(this, Codes.MODE_NORMAL);
    ProtocolObserver observer;
    private Context mainContext;
    private BluetoothIO bluetoothIO;
    private int ParkPos=0;
    private int Exit=0;
    private int Mode=0;

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
    // aici trebuie inceput un nou thread in care sa proceseze informatia...

        stateChanged(States.SEARCH_FOR_PARKING_SPOT); // starts looking for parking spots...


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
    public void sendCommand(ProtocolCommand command){

    }

    @Override
    public void responseArrived(ProtocolResponse response) {
        currentState.process(response);
    }

    @Override
    public void notifyNewInfoToShow(HMI_Info info) {

    }

    @Override
    public void stateChanged(States newState) {
        switch (newState){
            case IDLE_STATE: currentState = new Idle_State(this,Mode);
                break;
            case SEARCH_FOR_PARKING_SPOT: currentState = new SearchForParkingSpot_State(this,Mode);
                break;
            case TRY_PARKING: currentState = new TryParking_State(this,Mode);
                break;
            case STAY_STATIONARY: currentState = new StayStationary_State(this,Mode);
                break;
            case EXIT_PARKING_SPOT: currentState = new ExitParkingSpot_State(this,Mode);
                break;
            case SEARCH_FOR_EXIT: currentState = new SearchForExit_State(this);
                break;
            case EXIT_FOUND: currentState = new ExitFound_State(this);
                break;
            default: currentState = new Idle_State(this,Mode);
                break;
        }
    }
}
