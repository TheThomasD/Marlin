#pragma once

#include "yMultiple.hpp"

class yCircleButton : public yMultiple
{
    public:
        yCircleButton(int xStart = 0,int yStart = 0,int width = 0,int height = 0,const ScaleAttr* idle_attr = nullptr,const ScaleAttr* focus_attr = nullptr) :
        yScale(xStart,yStart,width,height,idle_attr,focus_attr)
		{
            circleR = (width > height ? width : height) / 2;
            circleDis = 1;
            circleX = wXStart + width / 2;
            circleY = wYStart + height / 2;
        }
        virtual ~yCircleButton(){}
        virtual uint8_t update(uint8_t force);
        inline virtual void show(uint8_t force = 0) {
            update(force);
            sign.content.scale = 0;
            sign.content.simulation = 0;
            sign.content.text = 0;
        }
        int circleX,circleY; //圆心坐标
        int circleR;//圆半径
        int circleDis;//圆周厚度
    private:
		void midCircle(color_t color,int r,int x,int y,int dis = 1);//画圆:圆心x,y,半径r,圆边厚度dis
		void pollCircle(color_t color_start,color_t color_end,int r,int dis,int xStart,int yStart,int xEnd,int yEnd);//画圆:半径r,厚度dis
};

class yRounded : public yMultiple //圆角矩形按钮
{
    public:
        yRounded(int xStart = 0,int yStart = 0,int width = 0,int height = 0,const ScaleAttr* idle_attr = nullptr,const ScaleAttr* focus_attr = nullptr) :
        yScale(xStart,yStart,width,height,idle_attr,focus_attr)
		{
            simW = width;
            simH = height;
            simR = 2;//默认圆角半径为2
        }
        virtual ~yRounded(){}
        void setSimulation(const PicAttr* idle,const PicAttr* focus) = delete;
        void setSimulationOffset(int x,int y) = delete;
        void setSimulationBgColor(color_t idle,color_t focus) = delete;
        virtual uint8_t update(uint8_t force);//层级:背景->圆角->文字(或多图)
        void setRounded(int x,int y,int width,int height,int rs = 1) {
            simOffsetX = x;
            simOffsetY = y;
            simW = width;
            simH = height;
            simR = rs;
        }

    private:
        void updateRounded(color_t color_start,color_t color_end);
        int simW,simH,simR;
};
