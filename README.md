HOW TO USE

// load a texture and a csv file to construct the tilemap...
sf::Texture& texture = Assets::getTexture("textures/attack.png");
TileSystem::Tilemap map(32, 10, 10, texture, "boi.csv");

// fill entire map with a specific ID
map.fill(0);

// set a specific coordinate to an ID
map.set(4, 4, -1);

// and make sure to draw
while (window.isOpen())
{
   window.clear(sf::Color::Black);
   window.draw(map);
   window.display();
}
