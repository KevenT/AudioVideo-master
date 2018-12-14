package edu.wuwang.ffmpeg.demo;

import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.support.annotation.Nullable;
import android.support.v7.app.AppCompatActivity;
import android.view.View;
import android.widget.TextView;

import java.io.ByteArrayOutputStream;
import java.io.Closeable;
import java.io.File;
import java.io.FileInputStream;
import java.io.IOException;
import java.io.RandomAccessFile;

import edu.wuwang.ffmpeg.FFMpeg;
import edu.wuwang.ffmpeg.R;
import edu.wuwang.ffmpeg.filter.YuvFilter;
import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import static java.lang.Thread.sleep;

/**
 * Created by aiya on 2017/4/5.
 */

public class H264DecoderDemo extends AppCompatActivity implements View.OnClickListener {

    private GLSurfaceView mGLView;
    private TextView mTvInfo;
    private FFMpeg mpeg;
    private YuvFilter mFilter;
    private byte[] data;
    private byte[] data1;
    private boolean isCodecStarted=false;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.demo_h264decoder);

        mGLView=(GLSurfaceView) findViewById(R.id.mGLView);
        mTvInfo=(TextView) findViewById(R.id.mTvInfo);
        FFMpeg.init();
        mpeg=new FFMpeg();
        mFilter=new YuvFilter(getResources());
        mGLView.setEGLContextClientVersion(2);
        mGLView.setPreserveEGLContextOnPause(true);
        mGLView.setRenderer(new GLSurfaceView.Renderer() {
            @Override
            public void onSurfaceCreated(GL10 gl, EGLConfig config) {
                mFilter.create();
            }

            @Override
            public void onSurfaceChanged(GL10 gl, int width, int height) {
                mFilter.setSize(width, height);
            }

            @Override
            public void onDrawFrame(GL10 gl) {
                if(isCodecStarted){
                    if(data==null||data.length!=mpeg.get(FFMpeg.KEY_WIDTH)*mpeg.get(FFMpeg.KEY_HEIGHT)){
                        data=new byte[mpeg.get(FFMpeg.KEY_WIDTH)*mpeg.get(FFMpeg.KEY_HEIGHT)*3/2];
                    }
                    if(mpeg.output(data)==0){
                        mFilter.updateFrame(mpeg.get(FFMpeg.KEY_WIDTH),mpeg.get(FFMpeg.KEY_HEIGHT),data);
                        mFilter.draw();
                    }
                }
            }
        });
//        mGLView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
        mGLView.setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if(mGLView!=null){
            mGLView.onResume();
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if(mGLView!=null){
            mGLView.onPause();
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()){
            case R.id.mBtnInfo:
//                mTvInfo.setText(FFMpeg.getInfo());

//                File file =  new File("/storage/emulated/0/1111test.h264");
//
////                byte[] by = readFile(file);
//                data1 = File2Bytes(file);
////                byte[]by ={01,01,02};
//                mpeg.input(data1);

                new Thread(new Runnable() {
                    @Override
                    public void run() {
//                        File file =  new File("/storage/emulated/0/1111test.h264");
//
//                        byte[] by = readFile(file);
//                        mpeg.input(by);

                        File file =  new File("/storage/emulated/0/test.h264");

//                byte[] by = readFile(file);
                        data1 = File2Bytes(file);
//                byte[]by ={01,01,02};
                        mpeg.input(data1,1024*1024);

                    }
                }).start();
                ;
                break;
            case R.id.mBtnStart:
                mpeg.start();
                isCodecStarted=true;
                break;
            case R.id.mBtnDecode:
                mGLView.requestRender();
                mGLView.setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);// 自动刷新
                break;
        }
    }

    public static byte[] readFile(File file) {
        RandomAccessFile rf = null;
        byte[] data = null;
        try {
            rf = new RandomAccessFile(file, "r");
            data = new byte[(int) rf.length()];
            rf.readFully(data);
        } catch (Exception exception) {
            exception.printStackTrace();
        } finally {
//            closeQuietly(rf);
        }
        return data;
    }
    //关闭读取file
    public static void closeQuietly(Closeable closeable) {
        try {
            if (closeable != null) {
                closeable.close();
            }
        } catch (Exception exception) {
            exception.printStackTrace();
        }
    }
    public static byte[] File2Bytes(File file) {
        int byte_size = 1024*1024;
        byte[] b = new byte[byte_size];
        try {
            FileInputStream fileInputStream = new FileInputStream(file);
            ByteArrayOutputStream outputStream = new ByteArrayOutputStream(
                    byte_size);
//            for (int length; (length = fileInputStream.read(b)) != -1;)
            {
                int length = fileInputStream.read(b);
                outputStream.write(b, 0, length);
//                Thread.sleep(3000);
            }
            fileInputStream.close();
            outputStream.close();
            return outputStream.toByteArray();
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return null;
    }

}
