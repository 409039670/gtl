package test.gtl.stil.storage.impl;

import gtl.stil.IndexSuits;
import gtl.stil.Identifier;
import gtl.stil.storage.StorageManager;
import gtl.stil.storage.impl.DiskStorageManager;
import junit.framework.Assert;
import junit.framework.TestCase;

import java.io.File;
import java.util.ArrayList;
import java.util.Iterator;

/**
 * Created by ZhenwenHe on 2016/12/10.
 */
public class DiskStorageManagerTest extends TestCase {



    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testLoadByteArray() throws Exception {

    }

    public void testStoreByteArray() throws Exception {
        ArrayList<Identifier> ids=new ArrayList<Identifier>();
        int pageSize =1024*8;//8k
        StorageManager dsm = IndexSuits.createDiskStorageManager("h:"+ File.separator+"test",pageSize,true);
        int dataSize=(int)(pageSize*2.6);
        byte [] data =new byte[dataSize];
        for(int i=0;i<10;i++){
            for(int j=0;j<dataSize;j++){
                data[j]=(byte)i;
            }
            Identifier pi = IndexSuits.createIdentifier(StorageManager.NEW_PAGE);
            dsm.storeByteArray(pi,data);
            ids.add((Identifier) pi.clone());
        }
        dsm.close();
        dsm = IndexSuits.createDiskStorageManager("h:"+ File.separator+"test",pageSize,false);
        Iterator<Identifier> it = ids.iterator();
        int k=0;
        while(it.hasNext()){
            data=null;
            Identifier id=it.next();
            System.out.println(id);
            data=dsm.loadByteArray(id);
            Assert.assertEquals(data.length,dataSize);
            System.out.println(data.length);
            System.out.println(data[0]);
            Assert.assertEquals(k,data[0]);
            System.out.println(data[data.length-1]);
            Assert.assertEquals(k,data[data.length-1]);
            k++;
        }
        dsm.close();
    }

    public void testDeleteByteArray() throws Exception {

    }

    public void testFlush() throws Exception {

    }

}