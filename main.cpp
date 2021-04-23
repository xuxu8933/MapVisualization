#include <iostream>
#include "openDriveReader.h"
#include "MapVisualization.h"
#include "stdafx.h"

using namespace ODReader;
int main() {
    float m_minX, m_minY, m_maxX, m_maxY;
    float m_pixelScaleX;
    /*! The pixel scale y coordinate */
    float m_pixelScaleY;
    std::cout << "Hello, World!" << std::endl;
    m_odReader = new ODReader::openDriveReader();
    if(m_odReader->FileReadErr>0)
    {
      //LOG_WARNING("File Read Error: %d", m_odReader->FileReadErr);
      return 0;
    }
    //Check the number of roads
    //LOG_INFO("Number of roads: %lu", m_odReader->RoadList.size());
    //Find the minimum and maximum X and y points
    m_minX = 99999999.0f, m_maxX = -99999999.0f;
    m_minY = 99999999.0f, m_maxY = -99999999.0f;
    float m_minZ = 99999999.0f, m_maxZ = -99999999.0f;
    int num = 5;//Number of points
    std::vector<ODReader::Pose3D> mapPoints = m_odReader->GetRoadPoints(m_odReader->RoadList,num);
    std::vector<ODReader::Pose3D> centerPoints = m_odReader->GetRoadPoints(m_odReader->RoadList,num,ODReader::CENTER_POINTS);
    std::vector<ODReader::Pose3D> borderPoints = m_odReader->GetRoadPoints(m_odReader->RoadList,num,ODReader::BORDER_POINTS);
    for (unsigned int i = 0; i < borderPoints.size(); i++)
    {
        float x = borderPoints[i].p.x;
        float y = borderPoints[i].p.y;
        float z = borderPoints[i].p.z;
        if (x < m_minX)
        {
            m_minX = x;
        }
        else if (x > m_maxX)
        {
            m_maxX = x;
        }
        if (y < m_minY)
        {
            m_minY = y;
        }
        else if (y > m_maxY)
        {
            m_maxY = y;
        }
        if (z < m_minZ)
        {
            m_minZ = z;
        }
        else if (z > m_maxZ)
        {
            m_maxZ = z;
        }
    }
    //Extend the map on either side for displaying points outside the map
    m_minX -= 0.3f;
    m_minY -= 0.3f;
    m_maxX += 0.3f;
    m_maxY += 0.3f;
    //Calculate scale from the fixed width and height
    m_pixelScaleX = GRAPHICSSCENE_WIDTH / fabs(m_maxX - m_minX);
    m_pixelScaleY = GRAPHICSSCENE_HEIGHT / fabs(m_maxY - m_minY);
    //Use the minimum scale of the two
    if (m_pixelScaleX < m_pixelScaleY)
    {
        m_pixelScaleY = m_pixelScaleX;
    }
    else {
        m_pixelScaleX = m_pixelScaleY;
    }
//    LOG_INFO("Min x %.3f Max X %.3f Scale X %.3f", m_minX, m_maxX, m_pixelScaleX);
//    LOG_INFO("Min Y %.3f Max Y %.3f Scale Y %.3f", m_minY, m_maxY, m_pixelScaleY);
//    LOG_INFO("Min Z %.3f Max Z %.3f", m_minZ, m_maxZ);
    for (unsigned int j = 0; j < mapPoints.size() - 1; j++)
    {
      //Convert map coordinates in -x1 to x2 to Pixel 0 to GRAPHICS_WIDTH
      //Convert map coordinates in y2 to -y1 to Pixel 0 to GRAPHICS_HEIGHT
      //Note Y direction is flipped as map and pixel is opposite in y-direction
      //Pixel coordinates
      float pixX1 = (mapPoints[j].p.x - m_minX)*m_pixelScaleX;
      float pixX2 = (mapPoints[j + 1].p.x - m_minX)*m_pixelScaleX;
      float pixY1 = GRAPHICSSCENE_HEIGHT - (mapPoints[j].p.y - m_minY)*m_pixelScaleY;
      float pixY2 = GRAPHICSSCENE_HEIGHT - (mapPoints[j + 1].p.y - m_minY)*m_pixelScaleY;
      float zScale = 0;
      if( fabs(m_maxZ-m_minZ)>0.01 )
      {
        zScale=255*(mapPoints[j].p.z - m_minZ)/fabs(m_maxZ-m_minZ);
      }
      //Send pixel coordinates to draw
      if( (j+1)%num !=0 && m_ShowLanes)
      {
        m_pDisplayWidget->DrawLine(pixX1, pixY1, pixX2, pixY2,zScale);
      }
    }
    for (unsigned int j = 0; j < centerPoints.size() - 1; j++)
    {
      //Convert map coordinates in -x1 to x2 to Pixel 0 to GRAPHICS_WIDTH
      //Convert map coordinates in y2 to -y1 to Pixel 0 to GRAPHICS_HEIGHT
      //Note Y direction is flipped as map and pixel is opposite in y-direction
      //Pixel coordinates
      float pixX1 = (centerPoints[j].p.x - m_minX)*m_pixelScaleX;
      float pixX2 = (centerPoints[j + 1].p.x - m_minX)*m_pixelScaleX;
      float pixY1 = GRAPHICSSCENE_HEIGHT - (centerPoints[j].p.y - m_minY)*m_pixelScaleY;
      float pixY2 = GRAPHICSSCENE_HEIGHT - (centerPoints[j + 1].p.y - m_minY)*m_pixelScaleY;
      //Send pixel coordinates to draw
      if( (j+1)%num !=0)
      {
        m_pDisplayWidget->DrawLine(pixX1, pixY1, pixX2, pixY2,0,CENTER_LANE);
      }
    }
    for (unsigned int j = 0; j < borderPoints.size() - 1; j++)
    {
      //Convert map coordinates in -x1 to x2 to Pixel 0 to GRAPHICS_WIDTH
      //Convert map coordinates in y2 to -y1 to Pixel 0 to GRAPHICS_HEIGHT
      //Note Y direction is flipped as map and pixel is opposite in y-direction
      //Pixel coordinates
      float pixX1 = (borderPoints[j].p.x - m_minX)*m_pixelScaleX;
      float pixX2 = (borderPoints[j + 1].p.x - m_minX)*m_pixelScaleX;
      float pixY1 = GRAPHICSSCENE_HEIGHT - (borderPoints[j].p.y - m_minY)*m_pixelScaleY;
      float pixY2 = GRAPHICSSCENE_HEIGHT - (borderPoints[j + 1].p.y - m_minY)*m_pixelScaleY;
      float zScale = 0;
      if( fabs(m_maxZ-m_minZ)>0.01 )
      {
        zScale=255*(borderPoints[j].p.z - m_minZ)/fabs(m_maxZ-m_minZ);
      }
      //Send pixel coordinates to draw
      if( (j+1)%num !=0)
      {
        //LOG_INFO("Road %d Point %d Mod %d",m_odReader->RoadList[i].id,j,(j+1)%num);
        //LOG_INFO("x1 %.3f y1 %.3f x2 %.3f y2 %.3f",mapPoints[j].p.x,mapPoints[j].p.y,mapPoints[j+1].p.x,mapPoints[j+1].p.y);
        m_pDisplayWidget->DrawLine(pixX1, pixY1, pixX2, pixY2,zScale,BORDER_LANE);
      }
    }
    return 0;
}
