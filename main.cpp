// SearchPath.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>
#include <algorithm>
#include <string>
#include <cstring>
using namespace std;

#define ROWS 8
#define COLS 5
class Solution {
    int matrix[ROWS * COLS];
    // store the longest path in pathCache for each position
    vector<int> pathCache[ROWS * COLS];
    // store the longest path length in lengthCache for each position
    int lengthCache[ROWS * COLS];
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

public:

    Solution(int mat[])
    {
        memcpy(matrix, mat, sizeof(int) * ROWS * COLS);
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


int main()
{
    int mat[ROWS * COLS];

    std::fstream ifs;
    ifs.open("input.txt", std::fstream::in);
    cout << "input matrix: " << endl;
    for (int row = 0; row < ROWS; row++)
    {
        for (int col = 0; col < COLS; col++)
        {
            ifs >> mat[row * COLS + col];
            cout << mat[row * COLS + col] << "\t";
        }
        cout << endl;
    }
    cout << endl;
    ifs.close();

    Solution s(mat);
    vector<int> longestPath = s.getPath();
    cout << "Longest Path: " << endl;
    for (int i = 0; i < longestPath.size(); i++)
    {
        int row = longestPath[i] / 5;
        int col = longestPath[i] - 5 * row;
        cout << row + 1 << col + 1;
        if(i < longestPath.size() - 1)
            cout << "-";
    }
    cout << endl;
}