package test.gtl.shape.impl;

import gtl.math.geometry.Envelope;
import gtl.math.geometry.GeometrySuits;
import gtl.index.IndexSuits;
import gtl.shape.Region;
import gtl.shape.Shape;
import gtl.shape.ShapeSuits;
import junit.framework.TestCase;

/**
 * Created by ZhenwenHe on 2017/3/3.
 */
public class RegionImplTest extends TestCase {
    public void setUp() throws Exception {
        super.setUp();

    }

    public void tearDown() throws Exception {

    }

    public void testIntersectsShape() throws Exception {

    }

    public void testContainsShape() throws Exception {

    }

    public void testTouchesShape() throws Exception {

    }

    public void testGetCenter() throws Exception {

    }

    public void testGetDimension() throws Exception {

    }

    public void testGetMBR() throws Exception {

    }

    public void testGetArea() throws Exception {

    }

    public void testGetMinimumDistance() throws Exception {

    }

    public void testClone() throws Exception {
        Envelope[] envs= GeometrySuits.readEnvelopeFile(IndexSuits.DATA_DIR+"test2d100.envelopes");
        Region[] rs = new Region[envs.length];
        Region[] rs2 = new Region[envs.length];
        int i=0;
        for(Envelope e: envs) {
            System.out.println(e);
            rs[i]= ShapeSuits.createRegion(e.getLowCoordinates(),e.getHighCoordinates());
            i++;
        }
        i=0;
        for (Region r: rs){
            byte[] bs = r.storeToByteArray();
            rs2[i]=ShapeSuits.createRegion();
            rs2[i].loadFromByteArray(bs);
            assertTrue(rs2[i].getMBR().equals(envs[i]));
            i++;
        }

    }

    public void testIntersectsRegion() throws Exception {

    }

    public void testContainsRegion() throws Exception {

    }

    public void testTouchesRegion() throws Exception {

    }

    public void testGetMinimumDistance1() throws Exception {

    }

    public void testContainsPoint() throws Exception {

    }

    public void testTouchesPoint() throws Exception {

    }

    public void testIntersectsLineSegment() throws Exception {

    }

    public void testGetMinimumDistance2() throws Exception {

    }

    public void testGetIntersectingRegion() throws Exception {

    }

    public void testGetIntersectingArea() throws Exception {

    }

    public void testGetMargin() throws Exception {

    }

    public void testCombineRegion() throws Exception {

    }

    public void testCombinePoint() throws Exception {

    }

    public void testGetCombinedRegion() throws Exception {

    }

}