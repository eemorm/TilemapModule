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

                std::vector<int> parsed; // maintain a vector of what has already been parsed
                std::string line; // line in the csv, or a row of tiles

                // find correct map sizing based on the csv
                int detectedWidth = 0;
                int detectedHeight = 0;

                while (std::getline(file, line)) // get each line in the csv
                {
                    std::stringstream ss(line);
                    std::string cell;
                    int rowWidth = 0;

                    while (std::getline(ss, cell, ',')) // split based on commas and loop for each character
                    {
                        parsed.push_back(std::stoi(cell)); // convert cell to int and push to temporary ID vector
                        rowWidth++; // keep track of the maximum row width
                    }

                    if (rowWidth > detectedWidth) // save the width if not initialized yet
                        detectedWidth = rowWidth;

                    detectedHeight++; // increment the height to keep track of it to be saved
                }

                // finally initialize all needed variables
                width = detectedWidth;
                height = detectedHeight;
                tiles = std::move(parsed);

                return true; // if success then complete
            }
            Tilemap(int ts, sf::Texture& t, std::string fp) : tileSize(ts), texture(&t) { loadCSV(fp); }
            void set(int x, int y, int id) { tiles[y * width + x] = id; } // set a specific tile's ID
            void fill(int id) { std::fill(tiles.begin(), tiles.end(), id); } // file a specific ID of tile with a tile
            int& getTile(int x, int y) { return tiles[y * width + x]; } // get a specific tile in the tilemap
    };
}
