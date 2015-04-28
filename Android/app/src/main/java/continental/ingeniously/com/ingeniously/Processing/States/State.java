package continental.ingeniously.com.ingeniously.Processing.States;

import continental.ingeniously.com.ingeniously.IO.Protocol.ProtocolResponse;

/**
 * Created by chisa_000 on 4/28/2015.
 */
public abstract class State {

    public abstract void process(ProtocolResponse response);
    protected abstract void notifyStateSwitch(States newState);

}
