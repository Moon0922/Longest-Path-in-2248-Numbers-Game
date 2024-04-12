// Longest-Path-in-2248-Numbers-Game.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <opencv2/opencv.hpp>
#include "DN.h"
using namespace cv;
using namespace std;

#define ROWS 8
#define COLS 5

// Directions array representing the four possible movements (left_up, top, right_up, left, right, left_down, down, right_down)
int directions[8][2] = {
    {-1, -1},
    {-1, 0},
    {-1, 1},
    {0, -1},
    {0, 1},
    {1, -1},
    {1, 0},
    {1, 1}
};

class Solution {
    int matrix[ROWS * COLS];
    // store the longest path in pathCache for each position
    vector<int> pathCache[ROWS * COLS];
    // store the longest path length in lengthCache for each position
    int lengthCache[ROWS * COLS];
   
public:

    Solution(int mat[])
    {
        memcpy(matrix, mat, sizeof(int) * ROWS * COLS);
    }

    Solution(vector<int> mat)
    {
        for (int i = 0; i < ROWS * COLS; i++)
            matrix[i] = mat[i];
    }

    void init()
    {
        memset(lengthCache, 0, sizeof(int) * ROWS * COLS);
        for (int row = 0; row < ROWS; row++)
        {
            for (int col = 0; col < COLS; col++)
            {
                pathCache[row * COLS + col].clear();
            }
        }
    }
    // Depth-first search function to find the longest increasing path
    int dfs(int row, int col, vector<int> path)
    {
        int i, j;
        int index = row * COLS + col;

        // If it already computed this position and there is no duplicated cell in the path, return the cached value
        if (path.size() && pathCache[index].size())
        {
            for (i = 0; i < path.size(); i++)
            {
                for (j = 0; j < pathCache[index].size(); j++)
                    if (path[i] == pathCache[index][j])
                        break;
                if (j != pathCache[index].size())
                    break;
            }
            if (i == path.size())
            {
                for (j = 0; j < pathCache[index].size(); j++)
                    path.push_back(pathCache[index][j]);

                if (pathCache[path[0]].size() < path.size())
                    pathCache[path[0]] = path;
                return pathCache[index].size();
            }
        }

        lengthCache[index] = 0;
        path.push_back(index);

        // Check all four possible directions
        for (int k = 0; k < 8; k++)
        {
            int newRow = row + directions[k][0];
            int newCol = col + directions[k][1];
            int newIndex = newRow * COLS + newCol;

            // Check if the new cell is within bounds and has the same value as the current cell or twice the current cell's value.  
            if (newRow >= 0 &&
                newRow < ROWS &&
                newCol >= 0 &&
                newCol < COLS &&
                (matrix[newIndex] == matrix[index]
                    || matrix[newIndex] == matrix[index] * 2))
            {
                for (i = 0; i < path.size(); i++)
                {
                    if (path[i] == newIndex)
                        break;
                }
                if (i != path.size())
                    continue;

                // Recursively explore the new position and update the cache
                int nPathCount = dfs(newRow, newCol, path);
                if (lengthCache[index] < nPathCount)
                {
                    lengthCache[index] = nPathCount;
                }
            }
        }
        if (pathCache[path[0]].size() < path.size())
            pathCache[path[0]] = path;

        // After exploring all directions, increment the path length by 1 and return
        return ++lengthCache[index];
    }

    vector<int> getPath()
    {
        int longestPathCount = 0;
        vector<int> longestPath;
        init();

        // Search longest path iterating through each cell in the matrix
        for (int row = 0; row < ROWS; row++) {
            for (int col = 0; col < COLS; col++) {
                // Compute the longest path starting from (row, col)
                int index = row * COLS + col;
                vector<int> path;
                int pathCount = dfs(row, col, path);

                // Check if the first cell's value is the same as the second cell's value in the path
                if (longestPathCount < pathCount &&
                    pathCache[index].size() >= 2 &&
                    matrix[pathCache[index][0]] == matrix[pathCache[index][1]])
                {
                    longestPathCount = pathCount;
                    longestPath = pathCache[index];
                }
            }
        }

        return longestPath;
    }
};

void SortBlocks(vector<Rect> &rtBlocks, vector<int> &nValues) 
{
    for(int i = 0; i < rtBlocks.size(); i++)
        for (int j = i + 1; j < rtBlocks.size(); j++)
        {
            if (rtBlocks[i].y - rtBlocks[j].y > rtBlocks[i].height)
            {
                Rect tempRect; int tempValue;
                tempRect = rtBlocks[i]; tempValue = nValues[i];
                rtBlocks[i] = rtBlocks[j]; nValues[i] = nValues[j];
                rtBlocks[j] = tempRect; nValues[j] = tempValue;
            }
            else if (rtBlocks[i].x > rtBlocks[j].x)
            {
                Rect tempRect; int tempValue;
                tempRect = rtBlocks[i]; tempValue = nValues[i];
                rtBlocks[i] = rtBlocks[j]; nValues[i] = nValues[j];
                rtBlocks[j] = tempRect; nValues[j] = tempValue;
            }
        }
}

int main(int argc, char* argv[])
{
    void* hEngineHandle = DNEngineHandleCreate();
    
    Mat src;
    if (argc == 1)
        src = imread("input.jpg");
    else
        src = imread(argv[1]);

    if (src.empty())
        return 0;

    float fScale = 450.f / src.cols;
    resize(src, src, Size(450, src.rows * fScale));
    imshow("input", src);
    Mat src_gray;
    cvtColor(src, src_gray, COLOR_BGR2GRAY);

    Mat dst;
    morphologyEx(src, dst, 2, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
    Mat dst_gray;
    cvtColor(dst, dst_gray, COLOR_BGR2GRAY);
    GaussianBlur(dst_gray, dst_gray, Size(11, 11), 0);
    Mat canny;
    Canny(dst_gray, canny, 0, 128);
    dilate(canny, canny, getStructuringElement(MORPH_RECT, Size(2, 2)));

    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    findContours(canny, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<Rect> rtBlocks;
    vector<int> nValues;
    for (size_t j = 0; j < contours.size(); j++)
    {
        if (contourArea(contours[j]) < src.cols * src.rows / 70)
            continue;

        Rect rtBlock = boundingRect(contours[j]);
        rtBlocks.push_back(rtBlock);
        Mat ele;
        src_gray(rtBlock).copyTo(ele);
        int n = DNEngineProcess(hEngineHandle, ele.data, ele.cols, ele.rows);
        nValues.push_back(n);
    }

    if (nValues.size() != ROWS * COLS)
        return 0;

    SortBlocks(rtBlocks, nValues);
    Solution s(nValues);
    vector<int> longestPath = s.getPath();
    for (int j = 1; j < longestPath.size(); j++)
    {
        int curRow = longestPath[j] / 5;
        int curCol = longestPath[j] - 5 * curRow;
        int befRow = longestPath[j - 1] / 5;
        int befCol = longestPath[j - 1] - 5 * befRow;
        int k = 0;
        for (k = 0; k < 8; k++)
        {
            if (curRow == befRow + directions[k][0] && curCol == befCol + directions[k][1])
                break;
        }
        Point curCenter = Point(rtBlocks[longestPath[j]].x + rtBlocks[longestPath[j]].width / 2, rtBlocks[longestPath[j]].y + rtBlocks[longestPath[j]].height / 2);
        Point befCenter = Point(rtBlocks[longestPath[j - 1]].x + rtBlocks[longestPath[j - 1]].width / 2, rtBlocks[longestPath[j - 1]].y + rtBlocks[longestPath[j - 1]].height / 2);

        befCenter.x = befCenter.x + directions[k][1] * rtBlocks[longestPath[j - 1]].width / 4;
        befCenter.y = befCenter.y + directions[k][0] * rtBlocks[longestPath[j - 1]].height / 4;

        curCenter.x = curCenter.x + (-1) * directions[k][1] * rtBlocks[longestPath[j]].width / 4;
        curCenter.y = curCenter.y + (-1) * directions[k][0] * rtBlocks[longestPath[j]].height / 4;
        //Rect rtBlock = rtBlocks[longestPath[j]];
        rectangle(src, rtBlocks[longestPath[j]], Scalar(255, 255, 255), 2);
        arrowedLine(src,
            befCenter,
            curCenter,
            Scalar(255, 255, 255), 3);
        if (j == 1)
        {
            circle(src, befCenter, 3, Scalar(255, 255, 255), 3);
            circle(src, befCenter, 5, Scalar(0, 0, 255), 2);
            rectangle(src, rtBlocks[longestPath[0]], Scalar(255, 255, 255), 2);
        }
    }
    imshow("output", src);
    waitKey(0);
    
    DNEngineHandleDestroy(hEngineHandle);
    return 0;
}