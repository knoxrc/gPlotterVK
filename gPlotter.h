#pragma once

#include "gPlotterOptions.h"
#include <vector>
#include "SensorData.h"

using namespace std;

vector<double> APNGSerializeHeaderChunks(PNGImage* png) {
    double length, i, chunkLength;
    NumberReference* position;

    length = png->signature->size() + 12.0 + PNGHeaderLength() + 12.0 + PNGIDATLength(png) + 12.0 + 20.0 + 38.0;
    if (png->physPresent) {
        length = length + 4.0 + 4.0 + 1.0 + 12.0;
    }
    //data = new vector<double>(length);
    vector<double> data(length);
    position = CreateNumberReference(0.0);

    /* PNG Signature */
    cout << "PNG Signature" << endl;
    for (i = 0.0; i < png->signature->size(); i = i + 1.0) {
        WriteByte(&data, png->signature->at(i), position);
    }

    /* IHDR Header */
    cout << "IHDR Header" << endl;
    chunkLength = PNGHeaderLength();
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"IHDR"), position);
    Write4BytesBE(&data, png->ihdr->Width, position);
    Write4BytesBE(&data, png->ihdr->Height, position);
    WriteByte(&data, png->ihdr->BitDepth, position);
    WriteByte(&data, png->ihdr->ColourType, position);
    WriteByte(&data, png->ihdr->CompressionMethod, position);
    WriteByte(&data, png->ihdr->FilterMethod, position);
    WriteByte(&data, png->ihdr->InterlaceMethod, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);

    /* acTL */
    cout << "acTL Header" << endl;
    chunkLength = 8.0;
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"acTL"), position);
    Write4BytesBE(&data, 2.0, position); //Temp
    Write4BytesBE(&data, 0.0, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);

    /* fcTL */
    cout << "fcTL Header" << endl;
    chunkLength = 26.0;
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"fcTL"), position);
    Write4BytesBE(&data, 0.0, position);
    Write4BytesBE(&data, png->ihdr->Width, position);
    Write4BytesBE(&data, png->ihdr->Height, position);
    Write4BytesBE(&data, 0.0, position);
    Write4BytesBE(&data, 0.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 1.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 1.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 0.0, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);

    /* pHYs */
    if (png->physPresent) {
        cout << "pHYs" << endl;
        chunkLength = 4.0 + 4.0 + 1.0;
        Write4BytesBE(&data, chunkLength, position);
        WriteStringBytes(&data, toVector(L"pHYs"), position);

        Write4BytesBE(&data, png->phys->pixelsPerMeter, position);
        Write4BytesBE(&data, png->phys->pixelsPerMeter, position);
        WriteByte(&data, 1.0, position);
        /* 1 = pixels per meter */
        Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);
    }

    /* IDAT */
    cout << "IDAT" << endl;
    chunkLength = PNGIDATLength(png);
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"IDAT"), position);
    WriteByte(&data, png->zlibStruct->CMF, position);
    WriteByte(&data, png->zlibStruct->FLG, position);
    for (i = 0.0; i < png->zlibStruct->CompressedDataBlocks->size(); i = i + 1.0) {
        WriteByte(&data, png->zlibStruct->CompressedDataBlocks->at(i), position);
    }
    Write4BytesBE(&data, png->zlibStruct->Adler32CheckValue, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);
   

    return data;
}

vector<double> APNGSerializeFrameChunks(PNGImage* png) {
    double length, i, chunkLength;
    
    NumberReference* position;

    length = 38.0 + PNGIDATLength(png) + 16.0 + 12.0;
    if (png->physPresent) {
        length = length + 4.0 + 4.0 + 1.0 + 12.0;
    }

    vector<double> data(length);
    //data = new vector<double>(length);
    position = CreateNumberReference(0.0);

    /* fcTL */
    cout << "fcTL Header" << endl;
    chunkLength = 26.0;
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"fcTL"), position);
    Write4BytesBE(&data, 1.0, position); //Temp pos
    Write4BytesBE(&data, png->ihdr->Width, position);
    Write4BytesBE(&data, png->ihdr->Height, position);
    Write4BytesBE(&data, 0.0, position);
    Write4BytesBE(&data, 0.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 1.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 1.0, position);
    WriteByte(&data, 0.0, position);
    WriteByte(&data, 0.0, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);

    /* pHYs */
    if (png->physPresent) {
        chunkLength = 4.0 + 4.0 + 1.0;
        Write4BytesBE(&data, chunkLength, position);
        WriteStringBytes(&data, toVector(L"pHYs"), position);

        Write4BytesBE(&data, png->phys->pixelsPerMeter, position);
        Write4BytesBE(&data, png->phys->pixelsPerMeter, position);
        WriteByte(&data, 1.0, position);
        /* 1 = pixels per meter */
        Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);
    }

    /* fdAT */
    cout << "fdAT" << endl;
    chunkLength = PNGIDATLength(png) + 4.0;
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"fdAT"), position);
    Write4BytesBE(&data, 1.0, position); //Temp
    WriteByte(&data, png->zlibStruct->CMF, position);
    WriteByte(&data, png->zlibStruct->FLG, position);
    for (i = 0.0; i < png->zlibStruct->CompressedDataBlocks->size(); i = i + 1.0) {
        WriteByte(&data, png->zlibStruct->CompressedDataBlocks->at(i), position);
    }
    Write4BytesBE(&data, png->zlibStruct->Adler32CheckValue, position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - chunkLength - 4.0, chunkLength + 4.0), position);

    /* IEND */
    cout << "IEND" << endl;
    chunkLength = 0.0;
    Write4BytesBE(&data, chunkLength, position);
    WriteStringBytes(&data, toVector(L"IEND"), position);
    Write4BytesBE(&data, CRC32OfInterval(&data, position->numberValue - 4.0, 4.0), position);

    return data;
}

vector<double> ConvertToAPNGWithOptions(RGBABitmapImage* image, int frame, double colorType, bool setPhys, double pixelsPerMeter, double compressionLevel) {
    PNGImage* png;
    vector<double> pngData, * colorData;

    png = new PNGImage();

    png->signature = PNGSignature();

    png->ihdr = PNGHeader(image, colorType);

    png->physPresent = setPhys;
    png->phys = PysicsHeader(pixelsPerMeter);

    if (colorType == 6.0) {
        colorData = GetPNGColorData(image);
    }
    else {
        colorData = GetPNGColorDataGreyscale(image);
    }
    png->zlibStruct = ZLibCompressStaticHuffman(colorData, compressionLevel);

    if (frame == 1)
    {
        pngData = APNGSerializeHeaderChunks(png);
    }
    if (frame > 1)
    {
        pngData = APNGSerializeFrameChunks(png);
    }
    return pngData;
}

vector<double> ConvertToAPNG(RGBABitmapImage* image, int frame)
{
    return ConvertToAPNGWithOptions(image, frame, 6.0, false, 0.0, 0.001);
}


template <class xGridOption>
class gPlotter {
protected:
    xGridOption xGrid;
    GridLabelsBasic bGrid;
    bool success, drawLegendFlag;
    RGBABitmapImageReference* canvasReference;
    ScatterPlotSettings* settings;
    StringReference* errorMessage;

    ScatterPlotSeries** series; //dyn alloc
    unsigned char series_SIZE;

    double xMin, xMax, yMin, yMax;

    bool extendSeries(int new_size)
    {
        if (new_size > series_SIZE)
        {
            // allocate the new array and check the mem;
            ScatterPlotSeries** NEW_series = (ScatterPlotSeries**)calloc(sizeof(ScatterPlotSeries*), new_size);
            if (NEW_series == NULL)
            {
                cout << "Out of mem allocation the extended array" << endl;
                return false;
            }
            // copy across all the (pointers to) existing series;
            for (int i = 0; i < series_SIZE; i++)
            {
                NEW_series[i] = series[i];
                series[i] = NULL;
            }
            // deallocate the old array
            if (series != NULL)
            {
                free(series);
                series = NULL;
            }

            // set the new values'
            series = NEW_series;
            series_SIZE = new_size;
            // done
            return true;
        }
        return false;
    }
    vector<wchar_t>* stringToWVector(string S) {
        wstring wS = wstring(S.begin(), S.end());
        return toVector(wS.c_str());
    }
    void getLineColor(int nodeID, double& red, double& blue, double& green)
    {
        //switch (nodeID)
        //{
        //case 1:

        //}
        if ((nodeID % 6) == 0)
        {
            red = 1;
            blue = 0;
            green = 0;
        }
        if ((nodeID % 6) == 1)
        {
            red = 0;
            blue = 1;
            green = 0;
        }
        if ((nodeID % 6) == 2)
        {
            red = 0;
            blue = 0;
            green = 1;
        }
        if ((nodeID % 6) == 3)
        {
            red = 1;
            blue = 0;
            green = 1;
        }
        if ((nodeID % 6) == 4)
        {
            red = 1;
            blue = 1;
            green = 0;
        }
        if ((nodeID % 6) == 5)
        {
            red = 0;
            blue = 1;
            green = 1;
        }
    }

public:
    gPlotter() {
        canvasReference = CreateRGBABitmapImageReference();
        settings = GetDefaultScatterPlotSettings();
        errorMessage = new StringReference();
        settings->width = 1000;
        settings->height = 600;
        settings->autoBoundaries = true;
        settings->autoPadding = true;

        series_SIZE = 0;
        drawLegendFlag = false;
        success = false;

        xMin = 0;
        yMin = 0;
        xMax = 0;
        yMax = 0;
    }
    ~gPlotter() { FreeImage(); }

    bool DrawScatterPlotFromSettings() {
        double xLength, yLength, i, x, y, xPrev, yPrev, px, py, pxPrev, pyPrev, originX, originY, p, l, plot;
        Rectangle* boundaries;
        double xPadding, yPadding, originXPixels, originYPixels;
        double xPixelMin, yPixelMin, xPixelMax, yPixelMax, xLengthPixels, yLengthPixels, axisLabelPadding;
        NumberReference* nextRectangle, * x1Ref, * y1Ref, * x2Ref, * y2Ref, * patternOffset;
        bool prevSet, success;
        RGBA* gridLabelColor;
        RGBABitmapImage* canvas;
        vector<double>* xs, * ys;
        bool linearInterpolation;
        ScatterPlotSeries* sp;
        vector<double>* xGridPositions, * yGridPositions;
        StringArrayReference* xLabels, * yLabels;
        NumberArrayReference* xLabelPriorities, * yLabelPriorities;
        vector<Rectangle*>* occupied;
        vector<bool>* linePattern;
        bool originXInside, originYInside, textOnLeft, textOnBottom;
        double originTextX, originTextY, originTextXPixels, originTextYPixels, side;

        canvas = CreateImage(settings->width, settings->height, GetWhite());
        patternOffset = CreateNumberReference(0.0);

        success = ScatterPlotFromSettingsValid(settings, errorMessage);

        if (success) {

            boundaries = new Rectangle();
            ComputeBoundariesBasedOnSettings(settings, boundaries);
            xMin = boundaries->x1;
            yMin = boundaries->y1;
            xMax = boundaries->x2;
            yMax = boundaries->y2;

            /* If zero, set to defaults. */
            if (xMin - xMax == 0.0) {
                xMin = 0.0;
                xMax = 10.0;
            }

            if (yMin - yMax == 0.0) {
                yMin = 0.0;
                yMax = 10.0;
            }

            xLength = xMax - xMin;
            yLength = yMax - yMin;

            if (settings->autoPadding) {
                xPadding = floor(GetDefaultPaddingPercentage() * settings->width);
                yPadding = floor(GetDefaultPaddingPercentage() * settings->height);
            }
            else {
                xPadding = settings->xPadding;
                yPadding = settings->yPadding;
            }

            /* Draw title */
            DrawTextPB(canvas, floor(settings->width / 2.0 - GetTextWidth(settings->title) / 2.0), floor(yPadding / 3.0), settings->title, GetBlack());

            /* Draw grid */
            xPixelMin = xPadding;
            yPixelMin = yPadding;
            xPixelMax = settings->width - xPadding;
            yPixelMax = settings->height - yPadding;
            xLengthPixels = xPixelMax - xPixelMin;
            yLengthPixels = yPixelMax - yPixelMin;
            DrawRectangle1px(canvas, xPixelMin, yPixelMin, xLengthPixels, yLengthPixels, settings->gridColor);

            gridLabelColor = GetGray(0.5);

            xLabels = new StringArrayReference();
            xLabelPriorities = new NumberArrayReference();
            yLabels = new StringArrayReference();
            yLabelPriorities = new NumberArrayReference();
            //xGridPositions = ComputeGridLinePositions(xMin, xMax, xLabels, xLabelPriorities, settings->xLabelTimeType);
            xGridPositions = xGrid.ComputeGridLinePositions(xMin, xMax, xLabels, xLabelPriorities);
            yGridPositions = bGrid.ComputeGridLinePositions(yMin, yMax, yLabels, yLabelPriorities);

            if (settings->showGrid) {
                /* X-grid */
                for (i = 0.0; i < xGridPositions->size(); i = i + 1.0) {
                    x = xGridPositions->at(i);
                    px = MapXCoordinate(x, xMin, xMax, xPixelMin, xPixelMax);
                    DrawLine1px(canvas, px, yPixelMin, px, yPixelMax, settings->gridColor);
                }

                /* Y-grid */
                for (i = 0.0; i < yGridPositions->size(); i = i + 1.0) {
                    y = yGridPositions->at(i);
                    py = MapYCoordinate(y, yMin, yMax, yPixelMin, yPixelMax);
                    DrawLine1px(canvas, xPixelMin, py, xPixelMax, py, settings->gridColor);
                }
            }

            /* Compute origin information. */
            originYInside = yMin < 0.0 && yMax > 0.0;
            originY = 0.0;
            if (settings->xAxisAuto) {
                if (originYInside) {
                    originY = 0.0;
                }
                else {
                    originY = yMin;
                }
            }
            else {
                if (settings->xAxisTop) {
                    originY = yMax;
                }
                if (settings->xAxisBottom) {
                    originY = yMin;
                }
            }
            originYPixels = MapYCoordinate(originY, yMin, yMax, yPixelMin, yPixelMax);

            originXInside = xMin < 0.0 && xMax > 0.0;
            originX = 0.0;
            if (settings->yAxisAuto) {
                if (originXInside) {
                    originX = 0.0;
                }
                else {
                    originX = xMin;
                }
            }
            else {
                if (settings->yAxisLeft) {
                    originX = xMin;
                }
                if (settings->yAxisRight) {
                    originX = xMax;
                }
            }
            originXPixels = MapXCoordinate(originX, xMin, xMax, xPixelMin, xPixelMax);

            if (originYInside) {
                originTextY = 0.0;
            }
            else {
                originTextY = yMin + yLength / 2.0;
            }
            originTextYPixels = MapYCoordinate(originTextY, yMin, yMax, yPixelMin, yPixelMax);

            if (originXInside) {
                originTextX = 0.0;
            }
            else {
                originTextX = xMin + xLength / 2.0;
            }
            originTextXPixels = MapXCoordinate(originTextX, xMin, xMax, xPixelMin, xPixelMax);

            /* Labels */
            occupied = new vector<Rectangle*>(xLabels->stringArray->size() + yLabels->stringArray->size());
            for (i = 0.0; i < occupied->size(); i = i + 1.0) {
                occupied->at(i) = CreateRectangle(0.0, 0.0, 0.0, 0.0);
            }
            nextRectangle = CreateNumberReference(0.0);

            /* x labels */
            for (i = 1.0; i <= 5.0; i = i + 1.0) {
                textOnBottom = true;
                if (!settings->xAxisAuto && settings->xAxisTop) {
                    textOnBottom = false;
                }
                DrawXLabelsForPriority(i, xMin, originYPixels, xMax, xPixelMin, xPixelMax, nextRectangle, gridLabelColor, canvas, xGridPositions, xLabels, xLabelPriorities, occupied, textOnBottom);
            }

            /* y labels */
            for (i = 1.0; i <= 5.0; i = i + 1.0) {
                textOnLeft = true;
                if (!settings->yAxisAuto && settings->yAxisRight) {
                    textOnLeft = false;
                }
                DrawYLabelsForPriority(i, yMin, originXPixels, yMax, yPixelMin, yPixelMax, nextRectangle, gridLabelColor, canvas, yGridPositions, yLabels, yLabelPriorities, occupied, textOnLeft);
            }

            /* Draw origin line axis titles. */
            axisLabelPadding = 20.0;

            /* x origin line */
            if (originYInside) {
                DrawLine1px(canvas, Round(xPixelMin), Round(originYPixels), Round(xPixelMax), Round(originYPixels), GetBlack());
            }

            /* y origin line */
            if (originXInside) {
                DrawLine1px(canvas, Round(originXPixels), Round(yPixelMin), Round(originXPixels), Round(yPixelMax), GetBlack());
            }

            /* Draw origin axis titles. */
            DrawTextUpwards(canvas, 10.0, floor(originTextYPixels - GetTextWidth(settings->yLabel) / 2.0), settings->yLabel, GetBlack());
            DrawTextPB(canvas, floor(originTextXPixels - GetTextWidth(settings->xLabel) / 2.0), yPixelMax + axisLabelPadding, settings->xLabel, GetBlack());

            /* X-grid-markers */
            for (i = 0.0; i < xGridPositions->size(); i = i + 1.0) {
                x = xGridPositions->at(i);
                px = MapXCoordinate(x, xMin, xMax, xPixelMin, xPixelMax);
                p = xLabelPriorities->numberArray->at(i);
                l = 1.0;
                if (p == 1.0) {
                    l = 8.0;
                }
                else if (p == 2.0) {
                    l = 3.0;
                }
                side = -1.0;
                if (!settings->xAxisAuto && settings->xAxisTop) {
                    side = 1.0;
                }
                DrawLine1px(canvas, px, originYPixels, px, originYPixels + side * l, GetBlack());
            }

            /* Y-grid-markers */
            for (i = 0.0; i < yGridPositions->size(); i = i + 1.0) {
                y = yGridPositions->at(i);
                py = MapYCoordinate(y, yMin, yMax, yPixelMin, yPixelMax);
                p = yLabelPriorities->numberArray->at(i);
                l = 1.0;
                if (p == 1.0) {
                    l = 8.0;
                }
                else if (p == 2.0) {
                    l = 3.0;
                }
                side = 1.0;
                if (!settings->yAxisAuto && settings->yAxisRight) {
                    side = -1.0;
                }
                DrawLine1px(canvas, originXPixels, py, originXPixels + side * l, py, GetBlack());
            }

            /* Draw points */
            for (plot = 0.0; plot < settings->scatterPlotSeries->size(); plot = plot + 1.0) {
                sp = settings->scatterPlotSeries->at(plot);

                xs = sp->xs;
                ys = sp->ys;
                linearInterpolation = sp->linearInterpolation;

                x1Ref = new NumberReference();
                y1Ref = new NumberReference();
                x2Ref = new NumberReference();
                y2Ref = new NumberReference();
                if (linearInterpolation) {
                    prevSet = false;
                    xPrev = 0.0;
                    yPrev = 0.0;
                    for (i = 0.0; i < xs->size(); i = i + 1.0) {
                        x = xs->at(i);
                        y = ys->at(i);

                        if (prevSet) {
                            x1Ref->numberValue = xPrev;
                            y1Ref->numberValue = yPrev;
                            x2Ref->numberValue = x;
                            y2Ref->numberValue = y;

                            success = CropLineWithinBoundary(x1Ref, y1Ref, x2Ref, y2Ref, xMin, xMax, yMin, yMax);

                            if (success) {
                                pxPrev = floor(MapXCoordinate(x1Ref->numberValue, xMin, xMax, xPixelMin, xPixelMax));
                                pyPrev = floor(MapYCoordinate(y1Ref->numberValue, yMin, yMax, yPixelMin, yPixelMax));
                                px = floor(MapXCoordinate(x2Ref->numberValue, xMin, xMax, xPixelMin, xPixelMax));
                                py = floor(MapYCoordinate(y2Ref->numberValue, yMin, yMax, yPixelMin, yPixelMax));

                                if (aStringsEqual(sp->lineType, toVector(L"solid")) && sp->lineThickness == 1.0) {
                                    DrawLine1px(canvas, pxPrev, pyPrev, px, py, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"solid"))) {
                                    DrawLine(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"dashed"))) {
                                    linePattern = GetLinePattern1();
                                    DrawLineBresenhamsAlgorithmThickPatterned(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, linePattern, patternOffset, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"dotted"))) {
                                    linePattern = GetLinePattern2();
                                    DrawLineBresenhamsAlgorithmThickPatterned(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, linePattern, patternOffset, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"dotdash"))) {
                                    linePattern = GetLinePattern3();
                                    DrawLineBresenhamsAlgorithmThickPatterned(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, linePattern, patternOffset, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"longdash"))) {
                                    linePattern = GetLinePattern4();
                                    DrawLineBresenhamsAlgorithmThickPatterned(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, linePattern, patternOffset, sp->color);
                                }
                                else if (aStringsEqual(sp->lineType, toVector(L"twodash"))) {
                                    linePattern = GetLinePattern5();
                                    DrawLineBresenhamsAlgorithmThickPatterned(canvas, pxPrev, pyPrev, px, py, sp->lineThickness, linePattern, patternOffset, sp->color);
                                }
                            }
                        }

                        prevSet = true;
                        xPrev = x;
                        yPrev = y;
                    }
                }
                else {
                    for (i = 0.0; i < xs->size(); i = i + 1.0) {
                        x = xs->at(i);
                        y = ys->at(i);

                        if (x > xMin && x < xMax && y > yMin && y < yMax) {

                            x = floor(MapXCoordinate(x, xMin, xMax, xPixelMin, xPixelMax));
                            y = floor(MapYCoordinate(y, yMin, yMax, yPixelMin, yPixelMax));

                            if (aStringsEqual(sp->pointType, toVector(L"crosses"))) {
                                DrawPixel(canvas, x, y, sp->color);
                                DrawPixel(canvas, x + 1.0, y, sp->color);
                                DrawPixel(canvas, x + 2.0, y, sp->color);
                                DrawPixel(canvas, x - 1.0, y, sp->color);
                                DrawPixel(canvas, x - 2.0, y, sp->color);
                                DrawPixel(canvas, x, y + 1.0, sp->color);
                                DrawPixel(canvas, x, y + 2.0, sp->color);
                                DrawPixel(canvas, x, y - 1.0, sp->color);
                                DrawPixel(canvas, x, y - 2.0, sp->color);
                            }
                            else if (aStringsEqual(sp->pointType, toVector(L"circles"))) {
                                DrawCircle(canvas, x, y, 3.0, sp->color);
                            }
                            else if (aStringsEqual(sp->pointType, toVector(L"dots"))) {
                                DrawFilledCircle(canvas, x, y, 3.0, sp->color);
                            }
                            else if (aStringsEqual(sp->pointType, toVector(L"triangles"))) {
                                DrawTriangle(canvas, x, y, 3.0, sp->color);
                            }
                            else if (aStringsEqual(sp->pointType, toVector(L"filled triangles"))) {
                                DrawFilledTriangle(canvas, x, y, 3.0, sp->color);
                            }
                            else if (aStringsEqual(sp->pointType, toVector(L"pixels"))) {
                                DrawPixel(canvas, x, y, sp->color);
                            }
                        }
                    }
                }
            }

            canvasReference->image = canvas;
        }

        return success;
    }
    bool DrawScatterPlot(double width, double height, vector<double>* xs, vector<double>* ys) {
        ScatterPlotSettings* settings;
        bool success;

        settings = GetDefaultScatterPlotSettings();

        settings->width = width;
        settings->height = height;
        settings->scatterPlotSeries = new vector<ScatterPlotSeries*>(1.0);
        settings->scatterPlotSeries->at(0) = GetDefaultScatterPlotSeriesSettings();
        delete settings->scatterPlotSeries->at(0)->xs;
        settings->scatterPlotSeries->at(0)->xs = xs;
        delete settings->scatterPlotSeries->at(0)->ys;
        settings->scatterPlotSeries->at(0)->ys = ys;

        success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);

        return success;
    }
    void TestMapping(NumberReference* failures) {
        ScatterPlotSeries* series;
        ScatterPlotSettings* settings;
        RGBABitmapImageReference* imageReference;
        double x1, y1;
        StringReference* errorMessage;
        bool success;

        errorMessage = CreateStringReference(toVector(L""));

        series = GetDefaultScatterPlotSeriesSettings();

        series->xs = new vector<double>(5.0);
        series->xs->at(0) = -2.0;
        series->xs->at(1) = -1.0;
        series->xs->at(2) = 0.0;
        series->xs->at(3) = 1.0;
        series->xs->at(4) = 2.0;
        series->ys = new vector<double>(5.0);
        series->ys->at(0) = -2.0;
        series->ys->at(1) = -1.0;
        series->ys->at(2) = -2.0;
        series->ys->at(3) = -1.0;
        series->ys->at(4) = 2.0;
        series->linearInterpolation = true;
        series->lineType = toVector(L"dashed");
        series->lineThickness = 2.0;
        series->color = GetGray(0.3);

        settings = GetDefaultScatterPlotSettings();
        settings->width = 600.0;
        settings->height = 400.0;
        settings->autoBoundaries = true;
        settings->autoPadding = true;
        settings->title = toVector(L"x^2 - 2");
        settings->xLabel = toVector(L"X axis");
        settings->yLabel = toVector(L"Y axis");
        settings->scatterPlotSeries = new vector<ScatterPlotSeries*>(1.0);
        settings->scatterPlotSeries->at(0) = series;

        imageReference = CreateRGBABitmapImageReference();
        success = DrawScatterPlotFromSettings(imageReference, settings, errorMessage);

        AssertTrue(success, failures);

        if (success) {
            x1 = MapXCoordinateAutoSettings(-1.0, imageReference->image, series->xs);
            y1 = MapYCoordinateAutoSettings(-1.0, imageReference->image, series->ys);

            AssertEquals(x1, 180.0, failures);
            AssertEquals(y1, 280.0, failures);
        }
    }
    void TestMapping2(NumberReference* failures) {
        vector<double>* xs, * ys, * xs2, * ys2;
        double i, x, y, w, h, xMin, xMax, yMin, yMax;
        RGBABitmapImageReference* canvasReference;
        ScatterPlotSettings* settings;
        double points;
        double x1, y1;
        StringReference* errorMessage;
        bool success;

        errorMessage = CreateStringReference(toVector(L""));

        points = 300.0;
        w = 600.0 * 2.0;
        h = 300.0 * 2.0;
        xMin = 0.0;
        xMax = 150.0;
        yMin = 0.0;
        yMax = 1.0;

        xs = new vector<double>(points);
        ys = new vector<double>(points);
        xs2 = new vector<double>(points);
        ys2 = new vector<double>(points);

        for (i = 0.0; i < points; i = i + 1.0) {
            x = xMin + (xMax - xMin) / (points - 1.0) * i;
            /* points - 1d is to ensure both extremeties are included. */
            y = x / (x + 7.0);

            xs->at(i) = x;
            ys->at(i) = y;

            y = 1.4 * x / (x + 7.0) * (1.0 - (atan((x / 1.5 - 30.0) / 5.0) / 1.6 + 1.0) / 2.0);

            xs2->at(i) = x;
            ys2->at(i) = y;
        }

        settings = GetDefaultScatterPlotSettings();

        settings->scatterPlotSeries = new vector<ScatterPlotSeries*>(2.0);
        settings->scatterPlotSeries->at(0) = new ScatterPlotSeries();
        settings->scatterPlotSeries->at(0)->xs = xs;
        settings->scatterPlotSeries->at(0)->ys = ys;
        settings->scatterPlotSeries->at(0)->linearInterpolation = true;
        settings->scatterPlotSeries->at(0)->lineType = toVector(L"solid");
        settings->scatterPlotSeries->at(0)->lineThickness = 3.0;
        settings->scatterPlotSeries->at(0)->color = CreateRGBColor(1.0, 0.0, 0.0);
        settings->scatterPlotSeries->at(1) = new ScatterPlotSeries();
        settings->scatterPlotSeries->at(1)->xs = xs2;
        settings->scatterPlotSeries->at(1)->ys = ys2;
        settings->scatterPlotSeries->at(1)->linearInterpolation = true;
        settings->scatterPlotSeries->at(1)->lineType = toVector(L"solid");
        settings->scatterPlotSeries->at(1)->lineThickness = 3.0;
        settings->scatterPlotSeries->at(1)->color = CreateRGBColor(0.0, 0.0, 1.0);
        settings->autoBoundaries = false;
        settings->xMin = xMin;
        settings->xMax = xMax;
        settings->yMin = yMin;
        settings->yMax = yMax;
        settings->yLabel = toVector(L"");
        settings->xLabel = toVector(L"Features");
        settings->title = toVector(L"");
        settings->width = w;
        settings->height = h;

        canvasReference = CreateRGBABitmapImageReference();

        success = DrawScatterPlotFromSettings(canvasReference, settings, errorMessage);

        AssertTrue(success, failures);

        if (success) {
            x1 = MapXCoordinateBasedOnSettings(27.0, settings);
            y1 = MapYCoordinateBasedOnSettings(1.0, settings);

            AssertEquals(floor(x1), 292.0, failures);
            AssertEquals(y1, 60.0, failures);
        }
    }
    void ExampleRegression(RGBABitmapImageReference* image) {
        vector<wchar_t>* xsStr, * ysStr;
        vector<double>* xs, * ys, * xs2, * ys2;
        StringReference* errorMessage;
        bool success;
        ScatterPlotSettings* settings;

        errorMessage = CreateStringReference(toVector(L""));

        xsStr = toVector(L"20.1, 7.1, 16.1, 14.9, 16.7, 8.8, 9.7, 10.3, 22, 16.2, 12.1, 10.3, 14.5, 12.4, 9.6, 12.2, 10.8, 14.7, 19.7, 11.2, 10.1, 11, 12.2, 9.2, 23.5, 9.4, 15.3, 9.6, 11.1, 5.3, 7.8, 25.3, 16.5, 12.6, 12, 11.5, 17.1, 11.2, 12.2, 10.6, 19.9, 14.5, 15.5, 17.4, 8.4, 10.3, 10.2, 12.5, 16.7, 8.5, 12.2");
        ysStr = toVector(L"31.5, 18.9, 35, 31.6, 22.6, 26.2, 14.1, 24.7, 44.8, 23.2, 31.4, 17.7, 18.4, 23.4, 22.6, 16.4, 21.4, 26.5, 31.7, 11.9, 20, 12.5, 18, 14.2, 37.6, 22.2, 17.8, 18.3, 28, 8.1, 14.7, 37.8, 15.7, 28.6, 11.7, 20.1, 30.1, 18.2, 17.2, 19.6, 29.2, 17.3, 28.2, 38.2, 17.8, 10.4, 19, 16.8, 21.5, 15.9, 17.7");

        xs = StringToNumberArray(xsStr);
        ys = StringToNumberArray(ysStr);

        settings = GetDefaultScatterPlotSettings();

        settings->scatterPlotSeries = new vector<ScatterPlotSeries*>(2.0);
        settings->scatterPlotSeries->at(0) = new ScatterPlotSeries();
        settings->scatterPlotSeries->at(0)->xs = xs;
        settings->scatterPlotSeries->at(0)->ys = ys;
        settings->scatterPlotSeries->at(0)->linearInterpolation = false;
        settings->scatterPlotSeries->at(0)->pointType = toVector(L"dots");
        settings->scatterPlotSeries->at(0)->color = CreateRGBColor(1.0, 0.0, 0.0);

        /*OrdinaryLeastSquaresWithIntercept(); */
        xs2 = new vector<double>(2.0);
        ys2 = new vector<double>(2.0);

        xs2->at(0) = 5.0;
        ys2->at(0) = 12.0;
        xs2->at(1) = 25.0;
        ys2->at(1) = 39.0;

        settings->scatterPlotSeries->at(1) = new ScatterPlotSeries();
        settings->scatterPlotSeries->at(1)->xs = xs2;
        settings->scatterPlotSeries->at(1)->ys = ys2;
        settings->scatterPlotSeries->at(1)->linearInterpolation = true;
        settings->scatterPlotSeries->at(1)->lineType = toVector(L"solid");
        settings->scatterPlotSeries->at(1)->lineThickness = 2.0;
        settings->scatterPlotSeries->at(1)->color = CreateRGBColor(0.0, 0.0, 1.0);

        settings->autoBoundaries = true;
        settings->yLabel = toVector(L"");
        settings->xLabel = toVector(L"");
        settings->title = toVector(L"");
        settings->width = 600.0;
        settings->height = 400.0;

        success = DrawScatterPlotFromSettings(image, settings, errorMessage);
    }
    double test() {
        double z;
        vector<double>* gridlines;
        NumberReference* failures;
        StringArrayReference* labels;
        NumberArrayReference* labelPriorities;
        RGBABitmapImageReference* imageReference;
        vector<double>* xs, * ys;
        StringReference* errorMessage;
        bool success;

        failures = CreateNumberReference(0.0);
        errorMessage = CreateStringReference(toVector(L""));

        imageReference = CreateRGBABitmapImageReference();

        labels = new StringArrayReference();
        labelPriorities = new NumberArrayReference();

        z = 10.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 11.0, failures);

        z = 9.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 19.0, failures);

        z = 8.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 17.0, failures);

        z = 7.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 15.0, failures);

        z = 6.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 13.0, failures);

        z = 5.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 21.0, failures);

        z = 4.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 17.0, failures);

        z = 3.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 31.0, failures);

        z = 2.0;
        gridlines = bGrid.ComputeGridLinePositions(-z / 2.0, z / 2.0, labels, labelPriorities, 0);
        AssertEquals(gridlines->size(), 21.0, failures);

        xs = new vector<double>(5.0);
        xs->at(0) = -2.0;
        xs->at(1) = -1.0;
        xs->at(2) = 0.0;
        xs->at(3) = 1.0;
        xs->at(4) = 2.0;
        ys = new vector<double>(5.0);
        ys->at(0) = 2.0;
        ys->at(1) = -1.0;
        ys->at(2) = -2.0;
        ys->at(3) = -1.0;
        ys->at(4) = 2.0;
        success = DrawScatterPlot(imageReference, 800.0, 600.0, xs, ys, errorMessage);

        AssertTrue(success, failures);

        if (success) {
            success = DrawBarPlot(imageReference, 800.0, 600.0, ys, errorMessage);

            AssertTrue(success, failures);

            if (success) {
                TestMapping(failures);
                TestMapping2(failures);
            }
        }

        return failures->numberValue;
    }

    void SetTitle(string title_in) {
        settings->title = stringToWVector(title_in);
    }
    void SetXLabel(string x_in) {
        settings->xLabel = stringToWVector(x_in);
    }
    void SetYLabel(string y_in) {
        settings->yLabel = stringToWVector(y_in);
    }
    void AddData(int dataSet, vector<double>* x_val, vector<double>* y_val, string legend_in) {
        if (dataSet < 0)
            cout << "Data set must have a posetive identifier" << endl;
        if (dataSet <= series_SIZE)
            extendSeries(dataSet + 1);

        double red, green, blue;
        series[dataSet] = GetDefaultScatterPlotSeriesSettings();
        series[dataSet]->linearInterpolation = true;
        series[dataSet]->lineThickness = 2;

        series[dataSet]->xs = x_val;
        //cout << series[dataSet]->xs->size() << ", " << x_val->size() << endl;
        series[dataSet]->ys = y_val;
        //cout << series[dataSet]->ys->size() << ", " << y_val->size() << endl;
        series[dataSet]->lineType = toVector(L"solid");
        getLineColor(dataSet, red, blue, green);
        series[dataSet]->color = CreateRGBColor(red, green, blue);
        settings->scatterPlotSeries->push_back(series[dataSet]);
        series[dataSet]->legend = stringToWVector(legend_in);
    }


    // grid specifiec gridlines. 0 for basic, 1 for hours, 2 for days
    void PlotNetwork(BasicSensorNetwork& data, const int varnum, int grid, int starts[4], int ends[4], unsigned char nodesToPlot[], int numNodesToPlot)
    {
        unsigned char max_nodeID, month, day, hour, min;
        unsigned short int year;
        unsigned int max_pos;
        BasicSensorDataEntry testEntry;
        float val, sec;
        float sixty = 60;
        float ten = 10;
        float twentyfour = 24;
        double sum_time, red, blue, green;
        const int nodeNo = 100;
        string valName, unit;
        int emptyNodes = 0;


        

        int sampleYear_start = starts[0];
        int sampleMonth_start = starts[1];
        int sampleDay_start = starts[2];
        int sampleHour_start = starts[3];

        int sampleYear_end = ends[0];
        int sampleMonth_end = ends[1];
        int sampleDay_end = ends[2];
        int sampleHour_end = ends[3];
        //cout << sampleYear_start << ", " << sampleMonth_start << ", " << sampleDay_start << ", " << sampleDay_start << endl;
        //cout << sampleYear_end << ", " << sampleMonth_end << ", " << sampleDay_end << ", " << sampleDay_end << endl;

        max_nodeID = data.getNetworkSize() - 1;

        data.getDataFromNetwork(0, 0, testEntry, false);



        vector<double> y_val[nodeNo];
        vector<double> t_val[nodeNo]; //Dyn Alloc

        int monthdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

        for (unsigned char in_nodeID = 0; in_nodeID <= max_nodeID; in_nodeID++)
        {
            bool checkreq = false;
            for (int i = 0; i < numNodesToPlot; i++)
            {
                if (in_nodeID == nodesToPlot[i])
                {
                    checkreq = true;
                }
            }
            if (data.returnDataListSize(in_nodeID) > 0 && checkreq)
            {
                
                //cout << "NODE: " << (int)in_nodeID << " In set. Empty Nodes:" << emptyNodes << endl;
                bool start = false;
                for (unsigned int in_pos = 0; in_pos < data.returnDataListSize(in_nodeID); in_pos++)
                {
                    data.getDataFromNetwork(in_nodeID, in_pos, testEntry, false);
                    testEntry.dateNtime.date.getYear(year);
                    testEntry.dateNtime.date.getMonth(month);
                    testEntry.dateNtime.date.getDay(day);
                    testEntry.dateNtime.time.getHour(hour);

                    int iyear = (int)year;
                    int imonth = (int)month;
                    int iday = (int)day;
                    int ihour = (int)hour;

                    //cout << iyear << ", " << imonth << ", " << iday << ", " << ihour << endl;

                    if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
                    {
                        monthdays[1] = 29;
                    }

                    if (iyear >= sampleYear_start && (imonth >= sampleMonth_start || iyear > sampleYear_start) && (iday >= sampleDay_start || imonth > sampleMonth_start) && (ihour >= sampleHour_start || iday > sampleDay_start))
                    {
                        start = true;
                    }
                    if (iyear >= sampleYear_end && (imonth >= sampleMonth_end || iyear > sampleYear_end) && (iday >= sampleDay_end || imonth > sampleMonth_end) && (ihour >= sampleHour_end || iday > sampleDay_end))
                    {
                        start = false;
                    }
                    if (start)
                    {
                        if (grid == 1) 
                        {
                            testEntry.reading.getVal(varnum, val);
                            testEntry.dateNtime.time.getHour(hour);
                            testEntry.dateNtime.time.getMinute(min);
                            sum_time = min / sixty;
                            testEntry.dateNtime.time.getSecond(sec);
                            sec = sec / sixty / sixty;
                            day = day * twentyfour;
                            sum_time = day + hour + sum_time + sec;
                            y_val[in_nodeID - emptyNodes].push_back(val);
                            t_val[in_nodeID - emptyNodes].push_back(sum_time);
                        }
                        else if (grid == 2)
                        {
                            testEntry.reading.getVal(varnum, val);
                            testEntry.dateNtime.time.getHour(hour);
                            testEntry.dateNtime.time.getMinute(min);
                            min = min / sixty / twentyfour;
                            testEntry.dateNtime.time.getSecond(sec);
                            sec = sec / sixty / sixty / twentyfour;
                            float h = (float)hour / twentyfour;
                            double d = (double)day;
                            double m = 0;
                            for (int i = 0; i + 1 < month; i++)
                            {
                                m = m + (double)monthdays[i];
                            }
                            double y = double(year) * (double)1000;
                            sum_time = y + m + d + h + min + sec;
                            y_val[in_nodeID - emptyNodes].push_back(val);
                            t_val[in_nodeID - emptyNodes].push_back(sum_time);
                        }
                       
                    }
                    else if (iyear >= sampleYear_end && (imonth >= sampleMonth_end || iyear > sampleYear_end) && (iday >= sampleDay_end || imonth > sampleMonth_end) && (ihour >= sampleHour_end || iday > sampleDay_end))
                    {
                        //cout << "DAY: " << (int)day << ", NODE: " << (int)in_nodeID << endl;
                        //cout << "here" << endl;
                        AddData(in_nodeID - emptyNodes, &t_val[in_nodeID - emptyNodes], &y_val[in_nodeID - emptyNodes], "Node: " + to_string((int)in_nodeID));
                        //cout << "Adding Series." << endl;
                        break;

                    }
                }
            }
            if (data.returnDataListSize(in_nodeID) < 1 || checkreq == false)
            {
                emptyNodes++;
                //cout << "NODE: " << (int)in_nodeID << " Out of set. Empty Nodes:" << emptyNodes << endl;
            }
        }

        testEntry.reading.getName(varnum, valName);
        testEntry.reading.getUnit(varnum, unit);

        string titleS = valName + " vs Time";
        string yLabelS = valName + " (" + unit + ")";

       
        wstring wideTitle = wstring(titleS.begin(), titleS.end());
        wstring wide_yLabel = wstring(yLabelS.begin(), yLabelS.end());

        bool success;

        string xlbl;
        if (grid == 1)
        {
            xlbl = "Time (Hours)";
        }
        else if(grid == 2)
        {
            xlbl = "Time (Day-Month)";
        }

        SetTitle(titleS);
        SetXLabel(xlbl);
        SetYLabel(yLabelS);

        success = DrawScatterPlotFromSettings();

        cout << endl << endl << endl;



        double corr12 = computeCorrelation(0, 1);
        cout << "Correlation between nodes 1 and 2: " << corr12 << endl;
        double corr149 = computeCorrelation(0, 2);
        cout << "Correlation between nodes 1 and 50: " << corr149 << endl;
        double corr249 = computeCorrelation(2, 1);
        cout << "Correlation between nodes 2 and 50: " << corr249 << endl;
        double cov12 = computeCovariance(0, 1);
        cout << "Covariance between nodes 1 and 2: " << cov12 << endl;
        double cov149 = computeCovariance(0, 2);
        cout << "Covariance between nodes 1 and 50: " << cov149 << endl;
        double cov249 = computeCovariance(2, 1);
        cout << "Covariance between nodes 2 and 50: " << cov249 << endl;

        //Legend
        DrawLegend(true);

        string legend;
        wstring wLegend;

        PrintPlot(titleS);

        string PNGName = titleS + ".png";
        string filePath = "C:\\Users\\knoxr\\Desktop\\Dis Outputs\\";
        string filePathPNG = filePath + PNGName;

        wstring widefilePath = wstring(filePath.begin(), filePath.end());
        wstring wideFilePathPNG = wstring(filePathPNG.begin(), filePathPNG.end());
        //ShellExecute(GetDesktopWindow(), L"open", wideFilePathPNG.c_str(), NULL, widefilePath.c_str(), SW_SHOWNORMAL);



        
    }


    //create overloaded function that will do what the other adddata does but with sensorDateAndTime
    void AddData(int dataSet, vector<basicSensorDateAndTime>* x_val, vector<double>* y_val, string legend_in) {
        if (dataSet < 0)
            cout << "Data set must have a posetive identifier" << endl;
        if (dataSet <= series_SIZE)
            extendSeries(dataSet + 1);
        unsigned char max_nodeID, month, day, hour, min;
        float sec;
        unsigned short int year;
        vector<double> t_val;
        for (int i = 0; i < dataSet; i++)
        {
                    x_val[i]->date.getYear(year);
                    x_val[i]->date.getMonth(month);
                    x_val[i]->date.getDay(day);
                    x_val[i]->time.getHour(hour);
                    x_val[i]->time.getMinute(min);
                    x_val[i]->time.getSecond(sec);
                    double secd = (double)sec;
                    double yeard = (double)year;
                    double monthd = (double)month;
                    double dayd = (double)day;
                    double hourd = (double)hour;
                    double mind = (double)min;
                /* change this*/    double tval = 1;
                    t_val.push_back(tval);
        }
        AddData(dataSet, &t_val, y_val, legend_in);
    }
    
    void DrawLegend(bool drawLegendFlag_in) {

        drawLegendFlag = drawLegendFlag_in;
    }
    void PrintPlot(string PNGName) {
        double red, green, blue;


        success = DrawScatterPlotFromSettings();
        if (drawLegendFlag) {
            //DrawFilledRectangle(canvasReference->image, 900, 0, 200, 15 * (series_SIZE)+100, CreateRGBColor(1, 1, 1));

            for (unsigned char seriesNO = 0; seriesNO < series_SIZE; seriesNO++)
            {
                getLineColor(seriesNO, red, blue, green);
                DrawFilledCircle(canvasReference->image, 910, 65 + 15 * seriesNO, 2, CreateRGBColor(red, green, blue));
                DrawTextPB(canvasReference->image, 920, 60 + 15 * seriesNO, series[seriesNO]->legend, CreateRGBColor(0, 0, 0));
            }
        }

        PNGName = PNGName + ".png";
        
        if (success) {
            vector<double>* pngdata = ConvertToPNG(canvasReference->image);
            WriteToFile(pngdata, PNGName);
        }
        else {
            cerr << "Error: ";
            for (wchar_t c : *errorMessage->string) {
                wcerr << c;
            }
            cerr << endl;
        }
    }
    void FreeImage() {
        DeleteImage(canvasReference->image);
        series_SIZE = 0;
    }


    double computeCorrelation(int s1, int s2)
    {
        bool sucess;
        int size = settings->scatterPlotSeries->size();
        if (s1 >= size || s2 >= size)
        {
            sucess = false;
            cout << "Error, series requested does not exist" << endl;
            return 7;
        }

        ScatterPlotSeries* sp1;
        vector<double>* xs1, * ys1;
        sp1 = settings->scatterPlotSeries->at(s1);
        xs1 = sp1->xs;
        ys1 = sp1->ys;

        ScatterPlotSeries* sp2;
        vector<double>* xs2, * ys2, * ys3;
        sp2 = settings->scatterPlotSeries->at(s2);
        xs2 = sp2->xs;
        ys2 = sp2->ys;

        int size1 = xs1->size();
        int size2 = xs2->size();
        vector<double> nvec;
        if (size1 < size2)
        {
            ys3 = ys1;
            alignVectors(xs1, ys1, xs2, ys2, &nvec);
        }
        else
        {

            ys3 = ys2;
            alignVectors(xs2, ys2, xs1, ys1, &nvec);
        }

        int size3 = ys3->size();
        double sum_1 = 0, sum_2 = 0, sum_12 = 0;
        double squareSum_1 = 0, squareSum_2 = 0;
        for (int i = 0; i < size3; i++) {

            double n1 = nvec[i];
            double n2 = ys3->at(i);
            sum_1 = sum_1 + n1;
            sum_2 = sum_2 + n2;
            sum_12 = sum_12 + n1 * n2;
            squareSum_1 = squareSum_1 + n1 * n1;
            squareSum_2 = squareSum_2 + n2 * n2;
        }
        double num = (double)(size3 * sum_12 - sum_1 * sum_2);
        double den = (double)sqrt((size3 * squareSum_1 - sum_1 * sum_1) * (size3 * squareSum_2 - sum_2 * sum_2));
        double corr = num / den;
        //cout << "Correlation between series " << s1 << " and series " << s2 << " is: " << corr << endl;
        return corr;
    }

    bool alignVectors(vector<double>* smallerx, vector<double>* smallery, vector<double>* largerx, vector<double>* largery, vector<double>* nvec)
    {
        double sx;
        int lxc = 0;

       
        int ssize = smallerx->size();
        int lsize = largerx->size();
        //cout << ssize << ", " << lsize << endl;
        double lx1 = largerx->at(lxc);
        double lx2 = largerx->at(lxc + 1);
        for (int i = 0; i < ssize; i++)
        {
            sx = smallerx->at(i);
            while (sx >= lx2 && lxc + 2 < lsize)
            {
                lxc++;
                lx2 = largerx->at(lxc + 1);
            }
            lx1 = largerx->at(lxc);
            double y1, y2;
            y1 = largery->at(lxc);
            if (sx < lx1)
            {
                y2 = y1;
            }
            else
            {
                y2 = largery->at(lxc + 1);
            }
            if (sx > lx2)
            {
                y1 = y2;
            }

            double slope = (y2 - y1) / (lx2 - lx1);
            double yintercept = y1 - lx1 * slope;
            double ny = sx * slope + yintercept;
            nvec->push_back(ny);
        }
        if (nvec->size() == ssize)
        {
            return true;
        }
        else
        {
            cout << "Error, alligning vectors failed" << endl;
            return false;
        }
    }
 
    double computeCovariance(int s1, int s2)
    {

        int size = settings->scatterPlotSeries->size();
        if (s1 >= size || s2 >= size)
        {
            cout << "Error, series requested does not exist" << endl;
            return 0;
        }

        ScatterPlotSeries* sp1;
        vector<double>* xs1, * ys1;
        sp1 = settings->scatterPlotSeries->at(s1);
        xs1 = sp1->xs;
        ys1 = sp1->ys;

        ScatterPlotSeries* sp2;
        vector<double>* xs2, * ys2, * ys3;
        sp2 = settings->scatterPlotSeries->at(s2);
        xs2 = sp2->xs;
        ys2 = sp2->ys;

        int size1 = xs1->size();
        int size2 = xs2->size();
        //cout << size1 << ", " << size2 << endl;

        vector<double> nvec;

        if (size1 < size2)
        {
            ys3 = ys1;
            alignVectors(xs1, ys1, xs2, ys2, &nvec);
        }
        else
        {
            ys3 = ys2;
            alignVectors(xs2, ys2, xs1, ys1, &nvec);
        }

        int size3 = ys3->size();
        double sum1 = 0;
        double sum2 = 0;
        for (int i = 0; i < size3; i++)
        {
            sum1 = sum1 + ys3->at(i);
            sum2 = sum1 + nvec[i];
        }
        double mean1 = sum1 / (double)size3;
        double mean2 = sum2 / (double)size3;
        double sum = 0;
        for (int i = 0; i < size3; i++)
        {
            sum = sum + (ys3->at(i) - mean1) * (nvec[i] - mean2);
        }
        double one = 1;
        double cov = sum / ((double)size3 - one);
        //cout << "Cvariance between series " << s1 << " and series " << s2 << " is: " << cov << endl;
        return cov;
    }
};

class heatMapPlotter : public gPlotter<GridLabelsBasic> {
protected:
    vector<double>* xs;
    vector<double>* ys;
    vector<double>* values;
    vector<double>* values2;
    vector<string>* labels;
    double value_min, value_max;
    int in_size;
    bool vec_flag;

    vector<double> pngdata1;
    vector<double> pngdata2;
    int pngcount;
public:
    heatMapPlotter() {
        settings->autoBoundaries = false;

        settings->xMin = 0;
        settings->yMin = 0;
        settings->xMax = 10;
        settings->yMax = 10;
        value_min = 0;
        value_max = 10;
        in_size = 0;
        xs = {};
        ys = {};
        values = {};
        values2 = {};
        vec_flag = false;

        pngcount = 0;
    }
    void setLoc(vector<double>* x_loc, vector<double>* y_loc, vector<string>* labels_in,  int size, double x_max, double y_max)
    {
        settings->xMax = x_max;
        settings->yMax = y_max;
        xs = x_loc;
        ys = y_loc;
        in_size = size;
        labels = labels_in;
    }
    void setVal(vector<double>* val, double val_min, double val_max)
    {
        values = val;
        value_max = val_max;
        value_min = val_min;
    }
    void setVecVal(vector<double>* valx, vector<double>* valy, double val_min, double val_max)
    {
        values = valx;
        values2 = valy;
        value_max = val_max;
        value_min = val_min;
        vec_flag = true;
    }
    void PrintArrows(double arrowLength)
    {
        //double la = 0.2;
        double angle;
        double xo, yo, xa, ya, xb, yb;
        for (int i = 0; i < in_size; i++)
        {
            xo = 100 + xs->at(i) * 800 / (settings->xMax);
            yo = 540 - ys->at(i) * 480 / (settings->yMax);
            angle = tan(values2->at(i) / values->at(i));
            xa = arrowLength * cos(angle);
            ya = arrowLength * sin(angle);
            xb = 0.5 * (xa * cos(3.14159265358979323846 / 4) - ya * sin(3.14159265358979323846 / 4));
            yb = 0.5 * (xa * sin(3.14159265358979323846 / 4) + ya * cos(3.14159265358979323846 / 4));
            //s = pow((pow(values->at(i), 2) + pow(values2->at(i), 2)) / pow(arrowLength, 2),0.5);
            //xa = values->at(i) / s * 800 / (settings->xMax);
            //ya = values2->at(i) / s * 480 / (settings->yMax);
            DrawLine(canvasReference->image, xo, yo, xo + xa, yo + ya, 4, HeatMapColor((pow(pow(values->at(i),2)+ pow(values2->at(i), 2),0.5) - value_min) / (value_max - value_min)));
            //s = 3.14159265358979323846 / 4 - atan(ya / xa);
            //xb = (la * cos(s) * (xa / abs(xa))) * 800 / (settings->xMax);
            //yb = (la * sin(s) * (xa / abs(xa))) * 480 / (settings->yMax);
            DrawLine(canvasReference->image, xo + xa, yo + ya, xo + xa - xb, yo + ya + yb, 4, HeatMapColor((pow(pow(values->at(i), 2) + pow(values2->at(i), 2), 0.5) - value_min) / (value_max - value_min)));
            DrawLine(canvasReference->image, xo + xa, yo + ya, xo + xa - yb, yo + ya - xb, 4, HeatMapColor((pow(pow(values->at(i), 2) + pow(values2->at(i), 2), 0.5) - value_min) / (value_max - value_min)));
        }
        
    }
    void PrintPlot(string PNGName) {
        success = DrawScatterPlotFromSettings();
        pngcount++;
        if (success) {

            DrawTextPB(canvasReference->image, 910, 440, stringToWVector(to_string((int)value_min)), CreateRGBColor(0, 0, 0));
            DrawFilledRectangle(canvasReference->image, 910, 400, 50, 25, CreateRGBColor(0, 0, 1));
            DrawFilledRectangle(canvasReference->image, 910, 375, 50, 25, CreateRGBColor(0.25, 0, 1));
            DrawFilledRectangle(canvasReference->image, 910, 350, 50, 25, CreateRGBColor(0.50, 0, 1));
            DrawFilledRectangle(canvasReference->image, 910, 325, 50, 25, CreateRGBColor(0.75, 0, 1));
            DrawFilledRectangle(canvasReference->image, 910, 300, 50, 25, CreateRGBColor(1, 0, 1));
            DrawFilledRectangle(canvasReference->image, 910, 275, 50, 25, CreateRGBColor(1, 0, 0.75));
            DrawFilledRectangle(canvasReference->image, 910, 250, 50, 25, CreateRGBColor(1, 0, 0.50));
            DrawFilledRectangle(canvasReference->image, 910, 225, 50, 25, CreateRGBColor(1, 0, 0.25));
            DrawFilledRectangle(canvasReference->image, 910, 200, 50, 25, CreateRGBColor(1, 0, 0));
            DrawTextPB(canvasReference->image, 910, 185, stringToWVector(to_string((int)value_max)), CreateRGBColor(0, 0, 0));
            
            
            if (!vec_flag)
            {
                for (int i = 0; i < in_size; i++)
                {
                    DrawFilledCircle(canvasReference->image, 100 + xs->at(i) * 800 / (settings->xMax), 540 - ys->at(i) * 480 / (settings->yMax), 12, HeatMapColor((values->at(i) - value_min) / (value_max - value_min)));
                    //DrawTextPB(canvasReference->image, 100 + xs->at(i) * 800 / (settings->xMax), 540 - ys->at(i) * 480 / (settings->yMax), stringToWVector(labels->at(i)), CreateRGBColor(0, 0, 0));
                }
            }
            if (vec_flag)
                PrintArrows(0.35); //input?
        }

        PNGName = PNGName + ".png";

        if (success) {
            vector<double>* pngdata = ConvertToPNG(canvasReference->image);
            WriteToFile(pngdata, PNGName);
            //if (pngcount == 1)
            //    pngdata1 = ConvertToAPNG(canvasReference->image, pngcount);
            //if (pngcount > 1)
            //{
            //    pngdata2 = ConvertToAPNG(canvasReference->image, pngcount);
            //    pngdata1.insert(pngdata1.end(), pngdata2.begin(), pngdata2.end());
            //    WriteToFile(&pngdata1, PNGName);
            //}

        }
        else {
            cerr << "Error: ";
            for (wchar_t c : *errorMessage->string) {
                wcerr << c;
            }
            cerr << endl;
        }
    }
    RGBA* HeatMapColor(double i)
    {
        if(i <= 0.5)
            return CreateRGBColor(i*2, 0, 1);
        if(i > 0.5)
            return CreateRGBColor(1, 0, 2 - i*2); //check
    }
};

