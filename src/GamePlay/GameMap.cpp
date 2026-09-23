#include "GameMap.h"
#include "Wall.h"
#include "Water.h"
#include "Bush.h"
#include "Utils/WorldPosition.h"
GameMap::GameMap(const std::vector<std::vector<int>>& map, const std::shared_ptr<Renderer::Texture2D>& pTexture, const std::shared_ptr<Renderer::ShaderProgram>& pShader)
    :m_mapData(map), m_pShader(pShader), m_pTexture(pTexture)
{
    buildTiles();
}
std::vector<std::shared_ptr<Tile>>& GameMap::getBackgroundTiles(){
    return m_backgroundTiles;
}
std::vector<std::shared_ptr<Tile>>& GameMap::getForegroundTiles(){
    return m_foregroundTiles;
}
std::vector<std::shared_ptr<Tile>>& GameMap::getBlockingTiles() {
    return m_blockingTiles;
}
void GameMap::reset(){
    m_backgroundTiles.clear();
    m_foregroundTiles.clear();
    m_blockingTiles.clear();
    buildTiles();
}
void GameMap::buildTiles(){
    int rows, cols;
    rows = static_cast<int>(m_mapData.size());
    cols = rows > 0 ? static_cast<int>(m_mapData[0].size()) : 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            int tileType = m_mapData[row][col];
            if(tileType == 0){ 
                continue; 
            }
            if(tileType == 1){                
                auto wall = std::make_shared<Wall>(m_pTexture, m_pShader, worldPosition(col+1, rows-row));
                m_backgroundTiles.push_back(wall);
                m_blockingTiles.push_back(wall);
            }
            if(tileType == 2){                
                auto mWall = std::make_shared<Wall>(m_pTexture, m_pShader, worldPosition(col+1, rows-row), true);
                m_backgroundTiles.push_back(mWall);
                m_blockingTiles.push_back(mWall);
            }
            if(tileType == 3){                
                auto bush = std::make_shared<Bush>(m_pTexture, m_pShader, worldPosition(col+1, rows-row));
                m_foregroundTiles.push_back(bush);
            }
            if(tileType == 4){                
                auto water = std::make_shared<Water>(m_pTexture, m_pShader, worldPosition(col+1, rows-row));
                m_backgroundTiles.push_back(water);
                m_blockingTiles.push_back(water);
            }
        }
    }
}
