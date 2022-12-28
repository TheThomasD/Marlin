
#pragma once

#if TRONXY_UI
#include "0windows/yFigure.hpp"
class mFigure : public yFigure
{
	public:
		mFigure(int xStart = 0,int yStart = 0,int width = 0,int height = 0,const ScaleAttr* idle_attr = nullptr,const ScaleAttr* focus_attr = nullptr) :
		yScale(xStart,yStart,width,height,idle_attr,focus_attr)
		{
			info = nullptr;
			infoOffsetX = 0;
			infoOffsetY = 0;
		}
		virtual bool update(bool force = false);
		void setInfo(int n,const uint16_t* s = nullptr);
		inline void setInfoOffset(int x,int y) {
			infoOffsetX = x;
			infoOffsetY = y;
		}
		inline void setInfoColor(color_t fr,color_t bg) {
			infoFr = fr;
			infoBg = bg;
		}
		uint16_t *info;

	private:
		int infoOffsetX,infoOffsetY;
		color_t infoFr,infoBg;
		void updateInfo(bool force);
};
#endif

