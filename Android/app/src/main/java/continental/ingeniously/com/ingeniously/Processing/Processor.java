package continental.ingeniously.com.ingeniously.Processing;

/**
 * Created by chisa_000 on 4/25/2015.
 */
public interface Processor {

    void StartProcessing();
    void setParkPos(int parkPos);
    void setExit(int exit);
    void setMode(int mode);
    void reset();


}
