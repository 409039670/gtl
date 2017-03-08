package gtl.stil;


import gtl.storage.Serializable;

/**
 * Created by ZhenwenHe on 2016/12/6.
 */
public interface Statistics extends Serializable {
     long getReadTimes()   ;
     long getWriteTimes()  ;
     long getNodeNumber() ;
     long getDataNumber();
     void reset();
}
