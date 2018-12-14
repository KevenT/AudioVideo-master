//
// Created by aiya on 2017/4/5.
//

#include "H264StreamDecoder.h"
#include "libswscale/swscale.h"

extern "C"{
#include "libavutil/file.h"
}

//#include <libavformat/avio.h>
//#include <libavcodec/avcodec.h>
//#include <libavformat/avformat.h>


struct AVCodecContext *pAVCodecCtx_decoder = NULL;
struct AVCodec *pAVCodec_decoder;
struct AVPacket mAVPacket_decoder;
struct AVFrame *pAVFrame_decoder = NULL;
struct SwsContext* pImageConvertCtx_decoder = NULL;
struct AVFrame *pFrameYUV_decoder = NULL;
FILE *fp_open=NULL;


//#define INBUF_SIZE 4096
//size_t INBUF_SIZE = 6*1024*1024;
size_t INBUF_SIZE = 1024*1024;
AVIOContext *avio_ctx;


struct buffer_data {
    uint8_t *ptr; /* 文件中对应位置指针 */
    size_t size; ///< size left in the buffer /* 文件当前指针到末尾 */
};

struct buffer_data bd = { 0 };
struct buffer_data bd1 = { 0 };
struct buffer_data bd2 = { 0 };
struct buffer_data bd3 = { 0 };

int bd0SeakCursor = 0;
int bd1SeakCursor = 0;
int bd2SeakCursor = 0;
int bd3SeakCursor = 0;

int isInist = 0;
int afcIsInist = 0;

int _size = 4;
int _front=0;
int _rear=0;
int _readingNo=_size;
int _writeingNo=_size;

//int H264StreamDecoder::start() {
//
//////    const char * test="/storage/emulated/0/test.h264";
////    avFormatContext=avformat_alloc_context();
////
////
////    char filepath[]="/storage/emulated/0/test.h264";
////    fp_open=fopen(filepath,"rb+");
////    //Init AVIOContext
////    unsigned char *aviobuffer=(unsigned char *)av_malloc(32768);
////    AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);
////    avFormatContext->pb=avio;
////
////    int ret=avformat_open_input(&avFormatContext,NULL,NULL,NULL);
//////    av_log(NULL,AV_LOG_DEBUG," =test=");
//////    log(*test,"==test ====test====test====test");
////
//////    int ret=avformat_open_input(&avFormatContext,test,NULL,NULL);
////
////
////
////    if(ret!=0){
////        log(ret,"avformat_open_input");
////        return ret;
////    }
////    ret=avformat_find_stream_info(avFormatContext,NULL);
////    if(ret<0){
////        log(ret,"avformat_find_stream_info");
////        return ret;
////    }
////    avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
////    avCodecContext=avcodec_alloc_context3(avCodec);
////
////
////    ret=avcodec_open2(avCodecContext,avCodec,NULL);
////    if(ret!=0){
////        log(ret,"avcodec_open2");
////        return ret;
////    }
//
//
////    avPacket=av_packet_alloc();
////    av_init_packet(avPacket);
////    avFrame=av_frame_alloc();
////    width=avFormatContext->streams[0]->codecpar->width;
////    height=avFormatContext->streams[0]->codecpar->height;
////    yFrameSize= (size_t) (width * height);
////    uvFrameSize= yFrameSize>>2;
////    av_log(NULL,AV_LOG_DEBUG,"w,h,yframe,uvframe info:%d,%d,%d,%d",width,height,yFrameSize,uvFrameSize);
////    av_log(NULL,AV_LOG_DEBUG," start success");
////    return 0;
//
//
//
//
//
//    width=640;
//    height=480;
//    yFrameSize= (size_t) (width * height);
//    uvFrameSize= yFrameSize>>2;
//
//    char filepath[]="/storage/emulated/0/test.h264";
////    const AVCodec *codec;
//    AVCodecParserContext *parser;
////    AVCodecContext *c= NULL;
//    FILE *f;
////    AVFrame *frame;
//    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
//    uint8_t *data;
//    size_t   data_size;
//    int ret;
////    AVPacket *pkt;
//
////    filename    = argv[1];
////    outfilename = argv[2];
//    avPacket = av_packet_alloc();
//    av_init_packet(avPacket);
//    if (!avPacket)
//        exit(1);
//    /* set end of buffer to 0 (this ensures that no overreading happens for damaged MPEG streams) */
//    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
//    /* find the MPEG-1 video decoder */
//    avCodec = avcodec_find_decoder(AV_CODEC_ID_H264);
//    if (!avCodec) {
//        fprintf(stderr, "Codec not found\n");
//        exit(1);
//    }
//    parser = av_parser_init(avCodec->id);
//    if (!parser) {
//        fprintf(stderr, "parser not found\n");
//        exit(1);
//    }
//    avCodecContext = avcodec_alloc_context3(avCodec);
//    if (!avCodecContext) {
//        fprintf(stderr, "Could not allocate video codec context\n");
//        exit(1);
//    }
//    /* For some codecs, such as msmpeg4 and mpeg4, width and height
//       MUST be initialized there because this information is not
//       available in the bitstream. */
//    /* open it */
//    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
//        fprintf(stderr, "Could not open codec\n");
//        exit(1);
//    }
//    f = fopen(filepath, "rb");
//    if (!f) {
//        fprintf(stderr, "Could not open %s\n", filepath);
//        exit(1);
//    }
//    avFrame = av_frame_alloc();
//    if (!avFrame) {
//        fprintf(stderr, "Could not allocate video frame\n");
//        exit(1);
//    }
//
//    while (!feof(f)) {
//        /* read raw data from the input file */
//        data_size = fread(inbuf, 1, INBUF_SIZE, f);
//        if (!data_size)
//            break;
//        /* use the parser to split the data into frames */
//        data = inbuf;
//        while (data_size > 0) {
//            ret = av_parser_parse2(parser, avCodecContext, &avPacket->data, &avPacket->size,
//                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
//            if (ret < 0) {
//                fprintf(stderr, "Error while parsing\n");
//                exit(1);
//            }
//            data      += ret;
//            data_size -= ret;
////            if (pkt->size)
////                decode(avCodecContext, frame, pkt, outfilename);
//        }
//    }
//
//
//
//    /* flush the decoder */
////    decode(avCodecContext, frame, NULL, outfilename);
//
////    fclose(f);
////    av_parser_close(parser);
////    avcodec_free_context(&avCodecContext);
////    av_frame_free(&avFrame);
////    av_packet_free(&avPacket);
//
//    return 0;
//
//}


//Callback
int read_buffer(void *opaque, uint8_t *buf, int buf_size){


//    if(!feof(fp_open)){
//        int true_size=fread(buf,1,buf_size,fp_open);
////        printf( "=====read_buffer=====\n");
////        log(2,"=====start=====");
//        return true_size;
//    }else{
//        return -1;
//    }

//    struct buffer_data *bd = (struct buffer_data *)opaque;
//    buf_size = FFMIN(buf_size, bd->size);
//
//    printf("ptr:%p size:%zu\n", bd->ptr, bd->size);
//
//    /* copy internal buffer data to buf */
//    memcpy(buf, bd->ptr, buf_size);
//    bd->ptr  += buf_size;
//    bd->size -= buf_size;
//
//    return buf_size;




    int copyBuf_size=0;

    if (_readingNo ==_size&& _writeingNo ==_size){
        

        while (_writeingNo==_front){
            _front= (_front+1)%_size;
        }

        _readingNo = _front;









        int isHaveGive=0;
        if(_front==0){

            if(bd.size==0){
                _front= (_front+1)%_size;
            }else{
                copyBuf_size = FFMIN(buf_size, bd.size);

                printf("ptr:%p size:%zu\n", bd.ptr, bd.size);

                /* copy internal buffer data to buf */
                if (copyBuf_size > 0) {
                    memcpy(buf, bd.ptr, copyBuf_size);
//                    bd.ptr += copyBuf_size;
                    bd0SeakCursor += copyBuf_size;
                    bd.size -= copyBuf_size;
                    _front= (_front+1)%_size;
                    isHaveGive =1;
                }
            }
        }
       if(isHaveGive==0&&_front==1) {
           if(bd1.size==0){
               _front= (_front+1)%_size;
           }else {
               copyBuf_size = FFMIN(buf_size, bd1.size);

               printf("ptr:%p size:%zu\n", bd1.ptr, bd1.size);

               /* copy internal buffer data to buf */
               if (copyBuf_size > 0) {
                   memcpy(buf, bd1.ptr+bd1SeakCursor, copyBuf_size);
//                   bd1.ptr += copyBuf_size;
                   bd1SeakCursor += copyBuf_size;
                   bd1.size -= copyBuf_size;
                   _front = (_front + 1) % _size;
                   isHaveGive =1;
               }
           }
        }
         if(isHaveGive==0&&_front==2) {
           if(bd2.size==0){
               _front= (_front+1)%_size;
           }else {
               copyBuf_size = FFMIN(buf_size, bd2.size);

               printf("ptr:%p size:%zu\n", bd2.ptr, bd2.size);

               /* copy internal buffer data to buf */
               if (copyBuf_size > 0) {
                   memcpy(buf, bd2.ptr+bd2SeakCursor, copyBuf_size);
//                   bd2.ptr += copyBuf_size;
                   bd2SeakCursor += copyBuf_size;
                   bd2.size -= copyBuf_size;
                   _front = (_front + 1) % _size;
                   isHaveGive =1;
               }
           }
        }
         if(isHaveGive==0&&_front==3) {
           if(bd3.size==0){
               _front= (_front+1)%_size;
           }else {
               copyBuf_size = FFMIN(buf_size, bd3.size);

               printf("ptr:%p size:%zu\n", bd3.ptr, bd3.size);

               /* copy internal buffer data to buf */
               if (copyBuf_size > 0) {
                   memcpy(buf, bd3.ptr+bd3SeakCursor, copyBuf_size);
//                   bd3.ptr += copyBuf_size;
                   bd3SeakCursor += copyBuf_size;
                   bd3.size -= copyBuf_size;
                   _front = (_front + 1) % _size;
                   isHaveGive =1;
               }
           }
        }
        _readingNo = _size;
    }

    return copyBuf_size;


}

int H264StreamDecoder::start() {

//    unsigned char *avio_ctx_buffer = NULL;
////    size_t avio_ctx_buffer_size = 32768;
//    size_t avio_ctx_buffer_size = 1024*1024;
//
//    AVInputFormat* in_fmt = av_find_input_format("h264");
//
//
//
//
////    bd.ptr = buf;  /* will be grown as needed by the realloc above */
////    bd.size = len; /* no data at this point */
//
//
//
////    else{
////        memcpy(bd.ptr+bd.size, buf, len);
////    //    bd.ptr = buf;  /* will be grown as needed by the realloc above */
////        bd.size = bd.size+len; /* no data at this point */
////
////    }
//
//    avFormatContext = avformat_alloc_context();
//
//    avio_ctx_buffer = (unsigned char *)av_malloc(avio_ctx_buffer_size);
//
//    /* 读内存数据 */
//    avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, NULL, read_buffer, NULL, NULL);
//
//    avFormatContext->pb = avio_ctx;
//    avFormatContext->flags = AVFMT_FLAG_CUSTOM_IO;
//
//    /* 打开内存缓存文件, and allocate format context */
////    if (avformat_open_input(&avFormatContext, "", in_fmt, NULL) < 0)
////    {
////        fprintf(stderr, "Could not open input\n");
////        return -1;
////    }
//
//    int ret=avformat_open_input(&avFormatContext,NULL,in_fmt,NULL);
//
//    if(ret!=0){
//        log(ret,"avformat_open_input");
//        return ret;
//    }
//    ret=avformat_find_stream_info(avFormatContext,NULL);
//    if(ret<0){
//        log(ret,"avformat_find_stream_info");
//        return ret;
//    }
//    avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
//    avCodecContext=avcodec_alloc_context3(avCodec);
//
//
//    ret=avcodec_open2(avCodecContext,avCodec,NULL);
//    if(ret!=0){
//        log(ret,"avcodec_open2");
//        return ret;
//    }
//    avPacket=av_packet_alloc();
//    av_init_packet(avPacket);
//    avFrame=av_frame_alloc();
//    width=avFormatContext->streams[0]->codecpar->width;
//    height=avFormatContext->streams[0]->codecpar->height;
//    yFrameSize= (size_t) (width * height);
//    uvFrameSize= yFrameSize>>2;

    return 0;

//    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
//
//
//
//////    const char * test="/mnt/sdcard/test.264";
////    const char * test="/storage/emulated/0/test.h264";
////    avFormatContext=avformat_alloc_context();
//////    log(2,"=====start=====");
//////    printf( "=====start=====\n");
////    char filepath[]="/storage/emulated/0/test.h264";
//////    fp_open=fopen(filepath,"rb+");
//////
//////    int true_size=fread(inbuf,1,INBUF_SIZE,fp_open);
//    uint8_t *input_buffer =NULL ;
//    uint8_t *output_buffer=NULL ;
//    size_t buffer_size;
//    struct buffer_data bd = { 0 };
//
//////    int ret = av_file_map(test, &input_buffer, &buffer_size, 0, NULL);
////
////    int ret = av_file_map(test, &input_buffer, &buffer_size, 0, NULL);
////    if(ret < 0)
////    {
////        printf(" ");
////    }
//
///* fill opaque structure used by the AVIOContext read callback */
//    /* bd 是指经过文件映射之后的文件，并不是指需要缓存区 */
//    bd.ptr  = input_buffer;
//    bd.size = buffer_size;
//
//    int ret = 0;
//
//
//    /* 分配内存, 可以自己设置缓冲大小,这里设置的是4K */
//    output_buffer = (uint8_t *) av_malloc(INBUF_SIZE);
//    if (!output_buffer) {
//        ret = AVERROR(ENOMEM);
//
//    }
//
//
//
////Init AVIOContext
////    unsigned char *aviobuffer=(unsigned char *)av_malloc(32768);
////    AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);
//
//    AVIOContext *avio =avio_alloc_context(output_buffer, INBUF_SIZE,0,&bd,read_buffer,NULL,NULL);
//    avFormatContext->pb=avio;
//
//     ret=avformat_open_input(&avFormatContext,NULL,NULL,NULL);
////    av_log(NULL,AV_LOG_DEBUG," =test=");
////    log(*test,"==test ====test====test====test");
//
////    int ret=avformat_open_input(&avFormatContext,test,NULL,NULL);
//    if(ret!=0){
//        log(ret,"avformat_open_input");
//        return ret;
//    }
//    ret=avformat_find_stream_info(avFormatContext,NULL);
//    if(ret<0){
//        log(ret,"avformat_find_stream_info");
//        return ret;
//    }
//    avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
//    avCodecContext=avcodec_alloc_context3(avCodec);
//
//
//    ret=avcodec_open2(avCodecContext,avCodec,NULL);
//    if(ret!=0){
//        log(ret,"avcodec_open2");
//        return ret;
//    }
//    avPacket=av_packet_alloc();
//    av_init_packet(avPacket);
//    avFrame=av_frame_alloc();
//    width=avFormatContext->streams[0]->codecpar->width;
//    height=avFormatContext->streams[0]->codecpar->height;
//    yFrameSize= (size_t) (width * height);
//    uvFrameSize= yFrameSize>>2;
//    av_log(NULL,AV_LOG_DEBUG,"w,h,yframe,uvframe info:%d,%d,%d,%d",width,height,yFrameSize,uvFrameSize);
//    av_log(NULL,AV_LOG_DEBUG," start success");
////    log(ret,"start success");
//    return 0;
}



//int H264StreamDecoder::start1() {
////    const char * test="/mnt/sdcard/test.264";
//    const char * test="/storage/emulated/0/test.h264";
//    avFormatContext=avformat_alloc_context();
////    av_log(NULL,AV_LOG_DEBUG," =test=");
////    log(*test,"==test ====test====test====test");
//    int ret=avformat_open_input(&avFormatContext,test,NULL,NULL);
//    if(ret!=0){
//        log(ret,"avformat_open_input");
//        return ret;
//    }
//    ret=avformat_find_stream_info(avFormatContext,NULL);
//    if(ret<0){
//        log(ret,"avformat_find_stream_info");
//        return ret;
//    }
//    avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
//    avCodecContext=avcodec_alloc_context3(avCodec);
//
//
//    ret=avcodec_open2(avCodecContext,avCodec,NULL);
//    if(ret!=0){
//        log(ret,"avcodec_open2");
//        return ret;
//    }
//    avPacket=av_packet_alloc();
//    av_init_packet(avPacket);
//    avFrame=av_frame_alloc();
//    width=avFormatContext->streams[0]->codecpar->width;
//    height=avFormatContext->streams[0]->codecpar->height;
//    yFrameSize= (size_t) (width * height);
//    uvFrameSize= yFrameSize>>2;
//    av_log(NULL,AV_LOG_DEBUG,"w,h,yframe,uvframe info:%d,%d,%d,%d",width,height,yFrameSize,uvFrameSize);
//    av_log(NULL,AV_LOG_DEBUG," start success");
//    return 0;
//}


/* 打开前端传来的视频buffer */
int H264StreamDecoder::input(uint8_t *buf, int len)
//int H264StreamDecoder::input(uint8_t *buf)
{

    if (isInist!=0){



        while (_readingNo==_rear){
            _rear= (_rear+1)%_size;
        }
        _writeingNo =_rear;

        if(_rear==0){
            bd0SeakCursor = 0;
//            bd.ptr = buf;  /* will be grown as needed by the realloc above */
            memcpy( bd.ptr,buf, len);
            bd.size = len; /* no data at this point */
            _rear= (_rear+1)%_size;
        }
        else  if(_rear==1){
            bd1SeakCursor = 0;
//            bd1.ptr = buf;  /* will be grown as needed by the realloc above */
            memcpy( bd1.ptr,buf, len);
            bd1.size = len; /* no data at this point */
            _rear= (_rear+1)%_size;
        }
        else  if(_rear==2){
            bd2SeakCursor = 0;
//            bd2.ptr = buf;  /* will be grown as needed by the realloc above */
            memcpy( bd2.ptr,buf, len);
            bd2.size = len; /* no data at this point */
            _rear= (_rear+1)%_size;
        }
        else  if(_rear==3){
            bd3SeakCursor = 0;
//            bd3.ptr = buf;  /* will be grown as needed by the realloc above */
            memcpy( bd3.ptr,buf, len);
            bd3.size = len; /* no data at this point */
            _rear= (_rear+1)%_size;
        }

        _writeingNo =_size;

//        if (bd.size<=0){
//        if (_front==0){
//            bd.ptr = buf;  /* will be grown as needed by the realloc above */
//            bd.size = len; /* no data at this point */
//        }
////        else if (bd1.size<=0){
//        else if (_front==1){
//            bd1.ptr = buf;  /* will be grown as needed by the realloc above */
//            bd1.size = len; /* no data at this point */
////        } else if (bd2.size<=0){
//        } else if (_front==2){
//            bd2.ptr = buf;  /* will be grown as needed by the realloc above */
//            bd2.size = len; /* no data at this point */
////        } else if (bd3.size<=0){
//        } else if (_front==3){
//            bd3.ptr = buf;  /* will be grown as needed by the realloc above */
//            bd3.size = len; /* no data at this point */
//        }
    }else{
        isInist =1;

        unsigned char *avio_ctx_buffer = NULL;
    //    size_t avio_ctx_buffer_size = 32768;
        size_t avio_ctx_buffer_size = len;

        AVInputFormat* in_fmt = av_find_input_format("h264");


        bd.ptr = (uint8_t *)av_malloc(avio_ctx_buffer_size);
        bd1.ptr = (uint8_t *)av_malloc(avio_ctx_buffer_size);
        bd2.ptr = (uint8_t *)av_malloc(avio_ctx_buffer_size);
        bd3.ptr = (uint8_t *)av_malloc(avio_ctx_buffer_size);

        memcpy( bd.ptr,buf, len);

//            bd.ptr = buf;  /* will be grown as needed by the realloc above */
            bd.size = len; /* no data at this point */
            _rear= (_rear+1)%_size;

    //    else{
    //        memcpy(bd.ptr+bd.size, buf, len);
    //    //    bd.ptr = buf;  /* will be grown as needed by the realloc above */
    //        bd.size = bd.size+len; /* no data at this point */
    //
    //    }



        avFormatContext = avformat_alloc_context();

        avio_ctx_buffer = (unsigned char *)av_malloc(avio_ctx_buffer_size);

        /* 读内存数据 */
        avio_ctx = avio_alloc_context(avio_ctx_buffer, avio_ctx_buffer_size, 0, NULL, read_buffer, NULL, NULL);

        avFormatContext->pb = avio_ctx;
        avFormatContext->flags = AVFMT_FLAG_CUSTOM_IO;

        /* 打开内存缓存文件, and allocate format context */
    //    if (avformat_open_input(&avFormatContext, "", in_fmt, NULL) < 0)
    //    {
    //        fprintf(stderr, "Could not open input\n");
    //        return -1;
    //    }

        int ret=avformat_open_input(&avFormatContext,NULL,in_fmt,NULL);

        if(ret!=0){
            log(ret,"avformat_open_input");
            return ret;
        }
        ret=avformat_find_stream_info(avFormatContext,NULL);
        if(ret<0){
            log(ret,"avformat_find_stream_info");
            return ret;
        }
        avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
        avCodecContext=avcodec_alloc_context3(avCodec);


        ret=avcodec_open2(avCodecContext,avCodec,NULL);
        if(ret!=0){
            log(ret,"avcodec_open2");
            return ret;
        }
        avPacket=av_packet_alloc();
        av_init_packet(avPacket);
        avFrame=av_frame_alloc();
        width=avFormatContext->streams[0]->codecpar->width;
        height=avFormatContext->streams[0]->codecpar->height;
        yFrameSize= (size_t) (width * height);
        uvFrameSize= yFrameSize>>2;
        afcIsInist = 1;
    }


    return 0;
}



//int H264StreamDecoder::input(uint8_t * data) {
////    avPacket->data = data;
//
//    uint8_t *input_buffer =NULL ;
//    uint8_t *output_buffer=NULL ;
//    size_t buffer_size;
//
//
//
//    bd.ptr  = data;
////    bd.size = strlen((char *)data);
//    bd.size = 1024*1024;
//
//
//    int ret= 0;
//
//    /* 分配内存, 可以自己设置缓冲大小,这里设置的是4K */
//    output_buffer = (uint8_t *) av_malloc(INBUF_SIZE);
//    if (!output_buffer) {
//        ret = AVERROR(ENOMEM);
//
//    }
//
//    avFormatContext=avformat_alloc_context();
//
////Init AVIOContext
////    unsigned char *aviobuffer=(unsigned char *)av_malloc(32768);
////    AVIOContext *avio =avio_alloc_context(aviobuffer, 32768,0,NULL,read_buffer,NULL,NULL);
//
//    AVIOContext *avio =avio_alloc_context(output_buffer, INBUF_SIZE,0,&bd,read_buffer,NULL,NULL);
//    avFormatContext->pb=avio;
//
//    ret=avformat_open_input(&avFormatContext,NULL,NULL,NULL);
////    av_log(NULL,AV_LOG_DEBUG," =test=");
////    log(*test,"==test ====test====test====test");
//
////    int ret=avformat_open_input(&avFormatContext,test,NULL,NULL);
//    if(ret!=0){
//        log(ret,"avformat_open_input");
//        return ret;
//    }
//    ret=avformat_find_stream_info(avFormatContext,NULL);
//    if(ret<0){
//        log(ret,"avformat_find_stream_info");
//        return ret;
//    }
//    avCodec=avcodec_find_decoder(AV_CODEC_ID_H264);
//    avCodecContext=avcodec_alloc_context3(avCodec);
//
//
//    ret=avcodec_open2(avCodecContext,avCodec,NULL);
//    if(ret!=0){
//        log(ret,"avcodec_open2");
//        return ret;
//    }
//    avPacket=av_packet_alloc();
//    av_init_packet(avPacket);
//    avFrame=av_frame_alloc();
//    width=avFormatContext->streams[0]->codecpar->width;
//    height=avFormatContext->streams[0]->codecpar->height;
//    yFrameSize= (size_t) (width * height);
//    uvFrameSize= yFrameSize>>2;
//    av_log(NULL,AV_LOG_DEBUG,"w,h,yframe,uvframe info:%d,%d,%d,%d",width,height,yFrameSize,uvFrameSize);
//    av_log(NULL,AV_LOG_DEBUG," start success");
////    log(ret,"start success");
//    return 0;
//
//
//
//
//    return 0;
//}

int H264StreamDecoder::output(uint8_t *data) {
    int ret= 1;//av_read_frame(avFormatContext,avPacket);

    if(afcIsInist){
        ret= av_read_frame(avFormatContext,avPacket);
        if(ret!=0){
            log(ret,"av_read_frame");
            return ret;
        }
        ret=avcodec_send_packet(avCodecContext,avPacket);
        if(ret!=0){
            log(ret,"avcodec_send_packet");
            return ret;
        }
        ret=avcodec_receive_frame(avCodecContext,avFrame);
        if(ret==0){
            memcpy(data, avFrame->data[0], yFrameSize);
            memcpy(data+yFrameSize, avFrame->data[1], uvFrameSize);
            memcpy(data+yFrameSize+uvFrameSize, avFrame->data[2], uvFrameSize);
        }else{
            log(ret,"avcodec_receive_frame");
        }
        av_packet_unref(avPacket);
    }
    return ret;
}

int H264StreamDecoder::stop() {
    avcodec_free_context(&avCodecContext);
    avformat_close_input(&avFormatContext);
    return 0;
}

void H264StreamDecoder::set(int key, int value) {

}

int H264StreamDecoder::get(int key) {
    switch (key){
        case KEY_WIDTH:
            return width;
        case KEY_HEIGHT:
            return height;
        default:
            break;
    }
    return Codec::get(key);
}