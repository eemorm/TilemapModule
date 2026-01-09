#pragma once

// SFML
#include <SFML/Graphics.hpp>

// Standard Libraries
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

namespace TileSystem
{
    class Tilemap : public sf::Drawable
    {
        private:
            int tileSize; // size of each tile, or bit size
            int width, height; // width and height of the tilemap
            sf::Texture* texture; // reference to the texture for the tilemap, avoids copies
            std::unordered_map<int, sf::IntRect> rects;  // maps IDs to sections of the texture, or IntRects, in order to display the correct tile

            std::vector<int> tiles; // vector of tile IDs stored in a 1D vector for simple storage

            // draw method to draw the tilemap
            virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override 
            {
                sf::Sprite sprite; // declare sprite
                sprite.setTexture(*texture); // and set its texture

                for (int y = 0; y < height; y++) // go through the...
                {
                    for (int x = 0; x < width; x++) // tilemap...
                    {
                        int id = tiles[y * width + x]; // get the ID of the tile currently being viewed
                        if (id < 0) continue; // if tile is not IDed then stop viewing it

                        // splice the texture according to the ID
                        sprite.setTextureRect({
                            id * tileSize,
                            0,
                            tileSize,
                            tileSize
                        });

                        // set relative position to other tiles
                        sprite.setPosition(
                            static_cast<float>(x * tileSize),
                            static_cast<float>(y * tileSize)
                        );
                        target.draw(sprite, states); // draw the tile
                    }
                }
            }
        public:
            bool loadCSV(std::string& path)
            {
                std::ifstream file(path); // get csv file
                if (!file.is_open()) return false; // leave if not opened

                std::string line; // line in the csv, or a row of tiles
                int y = 0; // vertical position, or which row are you on

                while (std::getline(file, line) && y < height) // go through the csv
                {
                    std::stringstream ss(line);
                    std::string cell;
                    int x = 0;

                    while (std::getline(ss, cell, ',') && x < width) // go through the csv and get the ID numbers to assign them
                    {
                        tiles[y * width + x] = std::stoi(cell);
                        x++;
                    }
                    y++;
                }

                return true; // return true if success
            }
            Tilemap(int ts, int w, int h, sf::Texture& t) : tileSize(ts), width(w), height(h), texture(&t), tiles(w * h, -1) {}
            Tilemap(int ts, int w, int h, sf::Texture& t, std::string fp) : tileSize(ts), width(w), height(h), texture(&t), tiles(w * h, -1) { loadCSV(fp); }
            void set(int x, int y, int id) { tiles[y * width + x] = id; } // set a specific tile's ID
            void fill(int id) { std::fill(tiles.begin(), tiles.end(), id); } // file a specific ID of tile with a tile
            int& getTile(int x, int y) { return tiles[y * width + x]; } // get a specific tile in the tilemap
    };
}
