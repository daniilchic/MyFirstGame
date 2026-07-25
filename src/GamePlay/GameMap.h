#pragma once
#include "Tile.h"
#include "Water.h"
#include "Bush.h"
#include "Wall.h"
#include <vector>
#include <memory>
#include "Renderer/Texture2D.h"
#include "Renderer/ShaderProgram.h"

class GameMap {
    public:
        GameMap(const std::vector<std::vector<int>>& map, const std::shared_ptr<Renderer::Texture2D>& pTexture, const std::shared_ptr<Renderer::ShaderProgram>& pShader);
        ~GameMap() = default;
        
        std::vector<std::shared_ptr<Tile>>& getBackgroundTiles() ;
        std::vector<std::shared_ptr<Tile>>& getForegroundTiles() ;
        std::vector<std::shared_ptr<Tile>>& getBlockingTiles() ;
    private:
        std::vector<std::shared_ptr<Tile>> m_backgroundTiles;
        std::vector<std::shared_ptr<Tile>> m_foregroundTiles;
        std::vector<std::shared_ptr<Tile>> m_blockingTiles;
};
