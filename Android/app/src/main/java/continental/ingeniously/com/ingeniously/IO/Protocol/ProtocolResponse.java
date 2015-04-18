package continental.ingeniously.com.ingeniously.IO.Protocol;

import continental.ingeniously.com.ingeniously.IO.CRC16;

/**
 * Created by chisa_000 on 4/8/2015.
 */
public class ProtocolResponse {


    private byte Header = 0;
    private byte No_of_sensors = 0;
    private byte Info[];
    private short Time = 0;
    private short CRC = 0;

    // points to the next element in "array"
    private int Index = 0;


    public ProtocolResponse(){
        Header = 0;
        No_of_sensors = 0;
        Time = 0;
        CRC = 0;
        Index = 0;
    }

    public ProtocolResponse(ProtocolResponse copy){
        // call the functions to have all the checks done
        setHeader(copy.getHeader());
        setNo_of_sensors(copy.getNo_of_sensors());
        setInfo(copy.getInfo());
        setTime(copy.getTime());
        setCRC(copy.getCRC());
    }

    public void setHeader(byte header) {
        this.Header = header;
        Index = 1;
    }

    public byte getHeader() {
        return Header;
    }

    public byte getNo_of_sensors() {
        return No_of_sensors;
    }

    public void setNo_of_sensors(byte no_of_sensors) {
        if(no_of_sensors < 0){
            no_of_sensors = 0;
        }


        No_of_sensors = no_of_sensors;
        if(Info == null){
            Info = new byte[no_of_sensors];
        }
        Index = 2;
    }

    public byte[] getInfo() {
        return Info;
    }

    public void setInfo(byte[] info) {
        if(Info == null){
            Info = new byte[info.length];
            setNo_of_sensors((byte)info.length);
        }
        System.arraycopy( info, 0, Info, 0, info.length );
        Index = 2+No_of_sensors;
    }

    public void setInfo(byte info_i, int index) throws Exception{
        if(Info.length<=index){
            throw new Exception("Not enough space in Info array");
        }

        Info[index] = info_i;
        Index = 2 + index+1;
    }

    public short getTime() {
        return Time;
    }

    public void setTime(short time) {
        //This should be called only after No_of_sensors is set
        Time = time;
        Index = 3 + No_of_sensors +1;//+1 because there are 2 bytes
    }

    public void setTime_Low(byte low){
        //This should be called only after No_of_sensors is set
        this.Time |= low;
        Index = 3 + No_of_sensors +1;//+1 because this is the second byte
    }

    public void setTime_High(byte high){
        //This should be called only after No_of_sensors is set
        this.Time |= (short)(high<<8);
        Index = 3 + No_of_sensors;
    }

    public short getCRC() {
        return CRC;
    }

    public byte[] getBytes(){
        byte body[];
        if(getNo_of_sensors()>0){
            body =  new byte[getNo_of_sensors()+6]; // make space for the other things too
        }else {
            No_of_sensors = 0; //if it somehow is a negative number or 0
            body = new byte[7]; // 1 byte for everything except time and CRC
            body[2] = 0; //preset the value for No_of_sensors in body
        }

        body[0] = Header;
        body[1] = No_of_sensors;

        int i=0;
        for(i =0; i<No_of_sensors; i++){
            body[2+i] = Info[i];
        }

        i--; //i was incremented once without being used...

        body[3+i] = (byte)(Time>>8); //continue with i
        body[4+i] = (byte)(Time & 0xff);
        body[5+i] = (byte) (CRC >> 8);
        body[6+i] = (byte) (CRC & 0xff);

        return body;
    }

    private byte[] getBytes_no_CRC(){
        byte body[];
        if(getNo_of_sensors()>0){
         body =  new byte[getNo_of_sensors()+4]; // make space for the other things too
        }else {
         No_of_sensors = 0; //if it somehow is a negative number
         body = new byte[5]; // 1 byte for everything except time
         body[2] = 0; //preset the value for No_of_sensors in body
        }

        body[0] = Header;
        body[1] = No_of_sensors;

        int i=0;
        for(i =0; i<No_of_sensors; i++){
            body[2+i] = Info[i];
        }

        i--; //i was incremented once without being used...

        body[3+i] = (byte)(Time>>8); //continue with i
        body[4+i] = (byte)(Time & 0xff);

        return body;
    }

    public void setCRC_Low(byte low){
        //This should be called only after No_of_sensors is set
        CRC |= low & 0xff; // trebuie sa fac low & 0xff!!!!!! daca nu fac acest hocus pocus nu merge!!
        Index = 5 + No_of_sensors +1;//+1 because this is the second byte
    }

    public void setCRC_High(byte high){
        //This should be called only after No_of_sensors is set
        this.CRC |= (short)(high<<8);
        Index = 5 + No_of_sensors;
    }

    public void setCRC(byte[] crc) {
        if(crc.length  == 2) {
            System.arraycopy(crc, 0, CRC, 0, 2);
        }
        Index = 5 + No_of_sensors +1;//+1 because there are 2 bytes
    }

    public void setCRC(short crc) {
       this.CRC = crc;
       Index = 5 + No_of_sensors +1;//+1 because there are 2 bytes
    }

    public boolean verifyCRC(){
        if(CRC16.runCrc16Check(getBytes()) == 0){
            return  true;
        }
        return false;

    }

    public int getIndex() {
        return Index;
    }
}
