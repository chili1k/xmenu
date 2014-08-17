#import <ApplicationServices/ApplicationServices.h>
#import "draw.h"

bool
drawText(CGContextRef ctx, DrawCtx *drawCtx, CFStringRef itemName)
{
	CFAttributedStringRef attrItemName = mkAttrString(drawCtx, itemName);
	CTLineRef line = CTLineCreateWithAttributedString(attrItemName);
	CGRect lineRect = CTLineGetImageBounds(line, ctx);
	CGFloat h = CGRectGetHeight(lineRect);
	CGFloat w = CTLineGetTypographicBounds(line, NULL, NULL, NULL);
	CGFloat y = (drawCtx->h + drawCtx->font_siz) / 2 - h;
	if ((drawCtx->x + w) > drawCtx->w)
		return false;
	CGContextSetFillColorWithColor(ctx, drawCtx->sbg);
	CGContextFillRect(ctx, CGRectMake (drawCtx->x, 0, w, drawCtx->h));
	CGContextSetTextPosition(ctx, drawCtx->x, y);
	CTLineDraw(line, ctx);
	drawCtx->x += w;
	CFRelease(line);
	CFRelease(attrItemName);
	return true;
}

CFAttributedStringRef
mkAttrString(DrawCtx *drawCtx, CFStringRef str) {
	CFStringRef keys[] = { kCTFontAttributeName, kCTForegroundColorAttributeName };
	CFTypeRef values[] = { drawCtx->font, drawCtx->sfg };
	CFDictionaryRef attrs = CFDictionaryCreate(
		kCFAllocatorDefault,
		(const void **)&keys,
		(const void **)&values,
		sizeof keys / sizeof(CFStringRef),
		&kCFTypeDictionaryKeyCallBacks,
		&kCFTypeDictionaryValueCallBacks);
	return CFAttributedStringCreate(kCFAllocatorDefault, str, attrs);
}

CGColorRef
mkColor(char *hex_color)
{
	if (!hex_color || *hex_color != '#')
		return NULL;
	hex_color++;
	CGFloat rgba[4];
	rgba[3] = 1.0;
	int t;
	switch (strlen(hex_color)) {
	case 3:
		if (1 != sscanf(hex_color, "%3x", &t))
			return NULL;
		rgba[0] = (t >> 8) | ((t >> 4) & 0xF0);
		rgba[1] = ((t >> 4) & 0xF) | (t & 0xF0);
		rgba[2] = (t & 0xF) | ((t << 4) & 0xF0);
		break;
	case 6:
		if (1 != sscanf(hex_color, "%6x", &t))
			return NULL;
		rgba[0] = (t >> 16) & 0xFF;
		rgba[1] = (t >> 8) & 0xFF;
		rgba[2] = t & 0xFF;
		break;
	default:
		return NULL;
	}
	rgba[0] /= 255.0;
	rgba[1] /= 255.0;
	rgba[2] /= 255.0;
	return CGColorCreate(CGColorSpaceCreateDeviceRGB(), rgba);
}