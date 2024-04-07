#include <stdio.h>

#include <chrono>
#include <thread>

#ifdef __cplusplus
extern "C" {
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
#include <libavutil/time.h>
#include <libavutil/imgutils.h>
}
#endif

#include "RenderView.h"
#include "SDLApp.h"
#include "Timer.h"
#include "log.h"

#include <functional>

#include "AudioPlay.h"
#include "FFmpegPlayer.h"

struct RenderPairData
{
    RenderItem *item = nullptr;
    RenderView *view = nullptr;
};

static void FN_DecodeImage_Cb(unsigned char* data, int w, int h, void *userdata)
{
    RenderPairData *cbData = (RenderPairData*)userdata;

    // assign once
    if (!cbData->item) {
        cbData->item = cbData->view->createRGB24Texture(w, h);
    }

    ff_log_line("update texture");
    cbData->view->updateTexture(cbData->item, data, h);
    // std::this_thread::sleep_for(std::chrono::seconds(10));
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        ff_log_line("usage: %s media_file_path", "./ffmpeg-simple-player");
        return -1;
    }

    av_log_set_level(AV_LOG_VERBOSE);

    SDLApp a;

    // render video
    RenderView view;
    view.initSDL();

    // Timer ti;
    // std::function<void()> cb = bind(&RenderView::onRefresh, &view);
    // const int kRenderPresentMs = 10;
    // ti.start(&cb, kRenderPresentMs);

    RenderPairData *cbData = new RenderPairData;
    cbData->view = &view;

    FFmpegPlayer player;
    player.setFilePath(argv[1]);
    player.setImageCb(FN_DecodeImage_Cb, cbData);
    if (player.initPlayer() != 0) {
        return -1;
    }

    ff_log_line("FFmpegPlayer init success");

    player.start();

    return a.exec();
}
