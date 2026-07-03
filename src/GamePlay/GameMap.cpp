#include "GameMap.h"
#include "Wall.h"
#include "Water.h"
#include "Bush.h"
#include "Utils/WorldPosition.h"
GameMap::GameMap(const std::vector<std::vector<int>>& map, const std::shared_ptr<Renderer::Texture2D>& pTexture, const std::shared_ptr<Renderer::ShaderProgram>& pShader)
{
    int rows, cols;
    rows = map.size();
    cols = rows > 0 ? map[0].size() : 0;
    for(int row = 0; row < rows; row++){
        for(int col = 0; col < cols; col++){
            int tileType = map[row][col];
            if(tileType == 0){ 
                continue; 
            }
            if(tileType == 1){                
                auto wall = std::make_shared<Wall>(pTexture, pShader, worldPosition(col+1, rows-row));
                m_backgroundTiles.push_back(wall);
                m_blockingTiles.push_back(wall);
            }
            if(tileType == 2){                
                auto mWall = std::make_shared<Wall>(pTexture, pShader, worldPosition(col+1, rows-row), true);
                m_backgroundTiles.push_back(mWall);
                m_blockingTiles.push_back(mWall);
            }
            if(tileType == 3){                
                auto bush = std::make_shared<Bush>(pTexture, pShader, worldPosition(col+1, rows-row));
                m_foregroundTiles.push_back(bush);
            }
            if(tileType == 4){                
                auto water = std::make_shared<Water>(pTexture, pShader, worldPosition(col+1, rows-row));
                m_backgroundTiles.push_back(water);
                m_blockingTiles.push_back(water);
            }
        }
    }
}
const std::vector<std::shared_ptr<Tile>>& GameMap::getBackgroundTiles() const{
    return m_backgroundTiles;
}
const std::vector<std::shared_ptr<Tile>>& GameMap::getForegroundTiles() const{
    return m_foregroundTiles;
}
const std::vector<std::shared_ptr<Tile>>& GameMap::getBlockingTiles() const{
    return m_blockingTiles;
}
