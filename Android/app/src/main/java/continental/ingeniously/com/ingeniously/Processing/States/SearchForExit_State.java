package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/28/2015.
 */
public class SearchForExit_State extends State {
    StateObserver stateObserver;

    @Override
    public void process(ProtocolResponse response) {

    }

    public SearchForExit_State(StateObserver observer) {
        stateObserver = observer;
    }

    @Override
    protected void notifyStateSwitch(States newState) {
        stateObserver.stateChanged(newState);
    }
}
