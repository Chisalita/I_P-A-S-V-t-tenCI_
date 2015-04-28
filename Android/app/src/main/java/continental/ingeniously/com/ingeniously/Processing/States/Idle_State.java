package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/29/2015.
 */
public class Idle_State extends State{
    StateObserver stateObserver;

    public Idle_State(StateObserver observer, int mode) {
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
