package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/28/2015.
 */
public class StayStationary_State extends State {

    StateObserver stateObserver;

    @Override
    public void process(ProtocolResponse response) {

    }

    public StayStationary_State(StateObserver observer,int mode) {
        stateObserver = observer;
    }

    @Override
    protected void notifyStateSwitch(States newState) {
        stateObserver.stateChanged(newState);
    }
}
