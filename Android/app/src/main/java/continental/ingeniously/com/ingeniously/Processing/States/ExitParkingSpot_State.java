package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/28/2015.
 */
public class ExitParkingSpot_State extends State {
    StateObserver stateObserver;


    public ExitParkingSpot_State(StateObserver observer,int mode){
        stateObserver = observer;
    }

    @Override
    public void process(ProtocolResponse response) {

    }

    @Override
    protected void notifyStateSwitch(States newState) {
        stateObserver.stateChanged(newState);
    }
}
