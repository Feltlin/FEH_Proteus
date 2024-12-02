#ifndef LARGEMAP
#define LARGEMAP

#include "simulator_libraries/FEHLCD.h"
#include "simulator_libraries/FEHImages.h"

#include <algorithm>
#include <vector>
#include <string>
#include <random>
#include <map>
#include <array>

#include "./FastNoiseLite/FastNoiseLite.h"

#include "Player.h"

//Junhao wrote this entire class
class LargeMap{
    public:
        struct Tile{
            float type = 0;
        };

        // struct Item{
        //     int x, y;
        //     std::string type;
        // };

        // struct Mob{
        //     int x, y, health, damage;
        //     std::string type;
        // };

        struct Chunk{
            int x, y;
            struct Tile tile[16][16];
        };

        // struct Region{
        //     int x, y;
        //     struct Tile tile[16384][16384];
        // };

        // struct World{
        //     std::vector<std::vector<struct Chunk>> tileWorld;
        //     std::vector<std::vector<struct Region>> regionWorld;
        //     std::vector<struct Item> item;
        //     std::vector<struct Mob> mob;
        // };

        // struct World world;
        // float region[16384][16384];
        // float temp[16384][16384];

        FEHImage image;
        FastNoiseLite noise;
        std::vector<struct Chunk> loadedChunk;

        LargeMap(){
            // worldGeneration(0, 0, this->region, this->temp);
        };

        // void display(int sx, int sy){
        //     for(int i = 0; i < 20; ++i){
        //         for(int j = 0; j < 15; ++j){
        //             if(region[sx + i][sy + j] <= 0){
        //                 image.Open(tileID["Water"].data());
        //             }
        //             else{
        //                 image.Open(tileID["Stone"].data());
        //             }
        //             image.Draw(i * 16, (15 - j - 1) * 16);
        //             image.Close();
        //         }
        //     }
        // }

        void display(int px, int py) {
            // Ensure chunks around the player are generated
            nearbyChunk(px, py);

            // Player's chunk coordinates
            int pcx = px / 16;
            int pcy = py / 16;

            // Player's position within the chunk
            int ptx = px % 16;
            int pty = py % 16;

            // Center of the screen
            int pScreenx = 20 / 2; // Player's screen x-coordinate
            int pScreeny = 15 / 2; // Player's screen y-coordinate

            // Top-left screen's world coordinates
            int startWorldX = px - pScreenx;
            int startWorldY = py - pScreeny;

            // Bottom-right screen's world coordinates
            int endWorldX = startWorldX + 20;
            int endWorldY = startWorldY + 15;

            // Iterate over all visible chunks
            for (Chunk& chunk : loadedChunk) {
                // Chunk's world coordinates
                int chunkWorldX = chunk.x * 16;
                int chunkWorldY = chunk.y * 16;

                // Check if the chunk intersects the visible screen
                if (chunkWorldX + 16 <= startWorldX || chunkWorldX >= endWorldX ||
                    chunkWorldY + 16 <= startWorldY || chunkWorldY >= endWorldY) {
                    continue; // Skip chunks outside the visible area
                }

                // Visible range of tiles in this chunk
                int startTileX = std::max(0, startWorldX - chunkWorldX);
                int startTileY = std::max(0, startWorldY - chunkWorldY);
                int endTileX = std::min(15, endWorldX - chunkWorldX - 1);
                int endTileY = std::min(15, endWorldY - chunkWorldY - 1);

                // Render tiles in the visible range
                for (int tileY = startTileY; tileY <= endTileY; ++tileY) {
                    for (int tileX = startTileX; tileX <= endTileX; ++tileX) {
                        // Screen coordinates for this tile
                        int screenX = (chunkWorldX + tileX) - startWorldX;
                        int screenY = (chunkWorldY + tileY) - startWorldY;

                        // Display the tile
                        displayTile(screenX, screenY, chunk.tile[tileX][tileY]);
                    }
                }
            }
        }

        void displayPixel(int px, int py) {
            // Ensure nearby chunks are generated
            nearbyChunk(px, py);

            // Screen dimensions in pixels/tiles
            const int screenWidth = 320;
            const int screenHeight = 240;

            // Chunk size in tiles
            const int chunkSize = 16;

            // Top-left corner of the screen in world coordinates
            int startWorldX = px - (screenWidth / 2);
            int startWorldY = py - (screenHeight / 2);

            // Iterate through each pixel (tile) on the screen
            for (int screenY = 0; screenY < screenHeight; ++screenY) {
                for (int screenX = 0; screenX < screenWidth; ++screenX) {
                    // World coordinates for the current screen pixel
                    int worldX = startWorldX + screenX;
                    int worldY = startWorldY + screenY;

                    // Chunk coordinates
                    int chunkX = worldX / chunkSize;
                    int chunkY = worldY / chunkSize;

                    // Tile position within the chunk
                    int tileX = worldX % chunkSize;
                    int tileY = worldY % chunkSize;

                    // Handle negative modulus (C++ '%' can yield negative values)
                    if (tileX < 0) tileX += chunkSize;
                    if (tileY < 0) tileY += chunkSize;

                    // Find the chunk in loadedChunk
                    bool chunkFound = false;
                    for (const Chunk& chunk : loadedChunk) {
                        if (chunk.x == chunkX && chunk.y == chunkY) {
                            // Draw the tile as a pixel
                            displayPixelTile(screenX, screenY, chunk.tile[tileX][tileY]);
                            chunkFound = true;
                            break;
                        }
                    }

                    // // If the chunk is not found, draw a default pixel (e.g., empty space)
                    // if (!chunkFound) {
                    //     displayPixelTile(screenX, screenY, 0); // Default type/color
                    // }
                }
            }
        }



        // void display(int px, int py){
        //     nearbyChunk(px, py);
        //     int pcx = px / 16;
        //     int pcy = py / 16;
        //     int ptx = px % 16;
        //     int pty = py % 16;
        //     int pScreenx = 20 / 2;
        //     int pScreeny = 15 / 2;

        //     int startChunkx = pcx - 1;
        //     int startChunky = pcy + 1;

        //     int startScreenx = px - pScreenx;
        //     int startScreeny = py + pScreeny;
        //     int endScreenx = startScreenx + 20;
        //     int endScreeny = startScreeny - 15;
        //     int startScreenChunkx = startScreenx / 16;
        //     int startScreenChunky = startScreeny / 16;
        //     int startScreenTilex = startScreenx % 16;
        //     int startScreenTiley = startScreeny % 16;
        //     for(struct Chunk chunk : loadedChunk){
        //         if(chunk.x == startScreenChunkx && chunk.y == startScreenChunky){
        //             for(int i = startScreenTilex; i < 16; ++i){
        //                 for(int j = startScreenTiley; j < 16; ++j){
        //                     displayTile(startScreenx + i * 16, startScreeny);
        //                 }
        //             }
        //         }
        //     }
        //     int endScreenChunkx = endScreenx / 16;
        //     int endScreenChunky = endScreeny / 16;
        //     int endScreenTilex = endScreenx % 16;
        //     int endScreenTiley = endScreeny % 16;
            
        // }

        void displayTile(int x, int y, struct Tile tile){
            if(tile.type > 0){
                image.Open("./Image/Stone.png");
            }
            else{
                image.Open("./Image/Water.png");
            }
            image.Draw(x * 16, y * 16);
            image.Close();
        }

        void displayPixelTile(int x, int y, struct Tile tile){
            LCD.SetFontColor((tile.type + 2) / 4 * 0xffffff);
            LCD.DrawPixel(x, y);
        }

        void nearbyChunk(int px, int py){
            int cx = px / 16;
            int cy = py / 16;
            for(int i = -1; i <= 1; ++i){
                for(int j = -1; j <= 1; ++j){
                    bool loaded = false;
                    for(struct Chunk chunk : loadedChunk){
                        if(chunk.x == cx + i && chunk.y == cy + j){
                            loaded = true;
                            break;
                        }
                    }
                    if(!loaded){
                        Chunk newChunk;
                        chunkGeneration(&newChunk, cx + i, cy + j);
                        loadedChunk.push_back(newChunk);
                    }                    
                }
            } 
        }

        void chunkGeneration(struct Chunk *chunk, int cx, int cy){
            noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
            chunk->x = cx;
            chunk->y = cy;
            for(int i = 0; i < 16; ++i){
                for(int j = 0; j < 16; ++j){
                    for(int n = 4096; n >= 1; n /= 2){
                        chunk->tile[i][j].type += noise.GetNoise(float(chunk->x * 16 + i)/n, float(chunk->y * 16 + j)/n) / float(n);
                    }
                }
            }
        }

        // void worldGeneration(int x, int y, float region[16384][16384], float temp[16384][16384]){
        //     //Island.
        //     noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
        //     noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
        //     float region4096[4][4];
        //     for(int i = 0; i < 4; ++i){
        //         for(int j = 0; j < 4; ++j){
        //             region4096[i][j] = noise.GetNoise((float)x, (float)y);;
        //         }
        //     }
        //     //Zoom 4096->2048.
        //     float region2048[8][8];
        //     for(int i = 0; i < 8; ++i){
        //         for(int j = 0; j < 8; ++j){
        //             //-1 to 1 random offset.
        //             region2048[i][j] = region4096[i/2][j/2] + rand()%3 - 1;
        //         }
        //     }
        //     //Add island.
        //     for(int i = 0; i < 8; ++i){
        //         for(int j = 0; j < 8; ++j){
        //             if(region2048[i][j] > 0){
        //                 //Count surrounding ocean tiles.
        //                 int oceanCount = 0;
        //                 for(int di = -1; di <= 1; ++di){
        //                     for(int dj = -1; dj <= 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 8 && j + dj >= 0 && j + dj < 8) {
        //                             if(region2048[i + di][j + dj] <= 0){
        //                                 ++oceanCount;
        //                             }
        //                         }
        //                     }
        //                 }
        //                 //5% erosion per ocean tile surrounded.
        //                 if(rand()%100 < oceanCount * 5){
        //                     region2048[i][j] -= 1;
        //                 }
        //                 for(int di = -1; di < 1; ++di){
        //                     for(int dj = -1; dj < 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 8 && j + dj >= 0 && j + dj < 8){
        //                             //30% expansion.
        //                             if(region2048[i + di][j + dj] <= 0 && rand()%10 < 3){
        //                                 region2048[i + di][j + dj] += 1;
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Zoom 2048->1024.
        //     float region1024[16][16];
        //     for(int i = 0; i < 16; ++i){
        //         for(int j = 0; j < 16; ++j){
        //             //-1 to 1 random offset.
        //             region1024[i][j] = region2048[i/2][j/2] + rand()%3 - 1;
        //         }
        //     }
        //     //Add island * 3.
        //     for(int i = 0; i < 16; ++i){
        //         for(int j = 0; j < 16; ++j){
        //             if(region1024[i][j] > 0){
        //                 //Count surrounding ocean tiles.
        //                 int oceanCount = 0;
        //                 for(int di = -1; di <= 1; ++di){
        //                     for(int dj = -1; dj <= 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 16 && j + dj >= 0 && j + dj < 16) {
        //                             if(region1024[i + di][j + dj] <= 0){
        //                                 ++oceanCount;
        //                             }
        //                         }
        //                     }
        //                 }
        //                 //5% erosion per ocean tile surrounded.
        //                 if(rand()%100 < oceanCount * 5){
        //                     region1024[i][j] -= 1;
        //                 }
        //                 for(int di = -1; di < 1; ++di){
        //                     for(int dj = -1; dj < 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 16 && j + dj >= 0 && j + dj < 16){
        //                             //30% expansion.
        //                             if(region1024[i + di][j + dj] <= 0 && rand()%10 < 3){
        //                                 region1024[i + di][j + dj] += 1;
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Add temperature.
        //     float temp1024[16][16];
        //     for(int i = 0; i < 16; ++i){
        //         for(int j = 0; j < 16; ++j){
        //             //Random temperature level 0 to 4.
        //             temp1024[i][j] = rand()%5;
        //         }
        //     }
        //     //Add island
        //     for(int i = 0; i < 16; ++i){
        //         for(int j = 0; j < 16; ++j){
        //             if(region1024[i][j] > 0){
        //                 //Count surrounding ocean tiles.
        //                 int oceanCount = 0;
        //                 for(int di = -1; di <= 1; ++di){
        //                     for(int dj = -1; dj <= 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 16 && j + dj >= 0 && j + dj < 16) {
        //                             if(region1024[i + di][j + dj] <= 0){
        //                                 ++oceanCount;
        //                             }
        //                         }
        //                     }
        //                 }
        //                 //5% erosion per ocean tile surrounded.
        //                 if(rand()%100 < oceanCount * 5){
        //                     region1024[i][j] -= 1;
        //                 }
        //                 for(int di = -1; di < 1; ++di){
        //                     for(int dj = -1; dj < 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 16 && j + dj >= 0 && j + dj < 16){
        //                             //30% expansion.
        //                             if(region1024[i + di][j + dj] <= 0 && rand()%10 < 3){
        //                                 region1024[i + di][j + dj] += 1;
        //                                 temp1024[i + di][j + dj] = temp1024[i][j];
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Add transition to temperature.
        //     for(int i = 0; i < 16; ++i){
        //         for(int j = 0; j < 16; ++j){
        //             for(int di = -1; di < 1; ++di){
        //                 for(int dj = -1; dj < 1; ++dj){
        //                     if(((di == 0 && (dj == -1 || dj == 1)) || (dj == 0 && (di == -1 || di == 1))) && i + di >= 0 && i + di < 8 && j + dj >= 0 && j + dj < 8){
        //                         //Temperature is warm(4).
        //                         if(temp1024[i][j] == 4){
        //                             //Adjacent temperature is freezing(0) or cold(1).
        //                             if(temp1024[i + di][j + dj] <= 1){
        //                                 //Change to temperate(2 or 3).
        //                                 temp1024[i][j] = 3;
        //                             }
        //                         }
        //                         //Temperature is freezing(0)
        //                         else if(temp1024[i][j] == 0){
        //                             //Adjacent temperature is warm(4) or temperate(2 or 3).
        //                             if(temp1024[i + di][j + dj] >= 2){
        //                                 //Change to cold(1).
        //                                 temp1024[i][j] = 1;
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Zoom 1024->512.
        //     float region512[32][32];
        //     float temp512[32][32];
        //     for(int i = 0; i < 32; ++i){
        //         for(int j = 0; j < 32; ++j){
        //             //-1 to 1 random offset.
        //             region512[i][j] = region1024[i/2][j/2] + rand()%3 - 1;
        //             temp512[i][j] = temp1024[i/2][j/2];
        //         }
        //     }
        //     //Zoom 512->256.
        //     float region256[64][64];
        //     float temp256[64][64];
        //     for(int i = 0; i < 64; ++i){
        //         for(int j = 0; j < 64; ++j){
        //             //-1 to 1 random offset.
        //             region256[i][j] = region512[i/2][j/2] + rand()%3 - 1;
        //             temp256[i][j] = temp512[i/2][j/2];
        //         }
        //     }
        //     //Add island
        //     for(int i = 0; i < 64; ++i){
        //         for(int j = 0; j < 64; ++j){
        //             if(region256[i][j] > 0){
        //                 //Count surrounding ocean tiles.
        //                 int oceanCount = 0;
        //                 for(int di = -1; di <= 1; ++di){
        //                     for(int dj = -1; dj <= 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 64 && j + dj >= 0 && j + dj < 64) {
        //                             if(region256[i + di][j + dj] <= 0){
        //                                 ++oceanCount;
        //                             }
        //                         }
        //                     }
        //                 }
        //                 //5% erosion per ocean tile surrounded.
        //                 if(rand()%100 < oceanCount * 5){
        //                     region256[i][j] -= 1;
        //                 }
        //                 for(int di = -1; di < 1; ++di){
        //                     for(int dj = -1; dj < 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 64 && j + dj >= 0 && j + dj < 64){
        //                             //30% expansion.
        //                             if(region256[i + di][j + dj] <= 0 && rand()%10 < 3){
        //                                 region256[i + di][j + dj] += 1;
        //                                 temp256[i + di][j + dj] = temp256[i][j];
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Zoom 256->128.
        //     float region128[128][128];
        //     float temp128[128][128];
        //     for(int i = 0; i < 128; ++i){
        //         for(int j = 0; j < 128; ++j){
        //             //-1 to 1 random offset.
        //             region128[i][j] = region256[i/2][j/2] + rand()%3 - 1;
        //             temp128[i][j] = temp256[i/2][j/2];
        //         }
        //     }
        //     //Zoom 128->64.
        //     float region64[256][256];
        //     float temp64[256][256];
        //     for(int i = 0; i < 256; ++i){
        //         for(int j = 0; j < 256; ++j){
        //             //-1 to 1 random offset.
        //             region64[i][j] = region128[i/2][j/2] + rand()%3 - 1;
        //             temp64[i][j] = temp128[i/2][j/2];
        //         }
        //     }
        //     //Zoom 64->32.
        //     float region32[512][512];
        //     float temp32[512][512];
        //     for(int i = 0; i < 512; ++i){
        //         for(int j = 0; j < 512; ++j){
        //             //-1 to 1 random offset.
        //             region32[i][j] = region64[i/2][j/2] + rand()%3 - 1;
        //             temp32[i][j] = temp64[i/2][j/2];
        //         }
        //     }
        //     //Add island
        //     for(int i = 0; i < 512; ++i){
        //         for(int j = 0; j < 512; ++j){
        //             if(region32[i][j] > 0){
        //                 //Count surrounding ocean tiles.
        //                 int oceanCount = 0;
        //                 for(int di = -1; di <= 1; ++di){
        //                     for(int dj = -1; dj <= 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 512 && j + dj >= 0 && j + dj < 512) {
        //                             if(region32[i + di][j + dj] <= 0){
        //                                 ++oceanCount;
        //                             }
        //                         }
        //                     }
        //                 }
        //                 //5% erosion per ocean tile surrounded.
        //                 if(rand()%100 < oceanCount * 5){
        //                     region32[i][j] -= 1;
        //                 }
        //                 for(int di = -1; di < 1; ++di){
        //                     for(int dj = -1; dj < 1; ++dj){
        //                         if(di == 0 && dj == 0){
        //                             continue;
        //                         }
        //                         if(i + di >= 0 && i + di < 512 && j + dj >= 0 && j + dj < 512){
        //                             //30% expansion.
        //                             if(region32[i + di][j + dj] <= 0 && rand()%10 < 3){
        //                                 region32[i + di][j + dj] += 1;
        //                                 temp32[i + di][j + dj] = temp32[i][j];
        //                             }
        //                         }
        //                     }
        //                 }
        //             }
        //         }
        //     }
        //     //Zoom 32->16.
        //     float region16[1024][1024];
        //     float temp16[1024][1024];
        //     for(int i = 0; i < 1024; ++i){
        //         for(int j = 0; j < 1024; ++j){
        //             //-1 to 1 random offset.
        //             region16[i][j] = region32[i/2][j/2] + rand()%3 - 1;
        //             temp16[i][j] = temp32[i/2][j/2];
        //         }
        //     }
        //     //Zoom 16->8.
        //     float region8[2048][2048];
        //     float temp8[2048][2048];
        //     for(int i = 0; i < 2048; ++i){
        //         for(int j = 0; j < 2048; ++j){
        //             //-1 to 1 random offset.
        //             region8[i][j] = region16[i/2][j/2] + rand()%3 - 1;
        //             temp8[i][j] = temp16[i/2][j/2];
        //         }
        //     }
        //     //Zoom 8->4.
        //     float region4[4096][4096];
        //     float temp4[4096][4096];
        //     for(int i = 0; i < 4096; ++i){
        //         for(int j = 0; j < 4096; ++j){
        //             //-1 to 1 random offset.
        //             region4[i][j] = region8[i/2][j/2] + rand()%3 - 1;
        //             temp4[i][j] = temp8[i/2][j/2];
        //         }
        //     }
        //     //Zoom 4->1.
        //     for(int i = 0; i < 16384; ++i){
        //         for(int j = 0; j < 16384; ++j){
        //             //-1 to 1 random offset.
        //             region[i][j] = region4[i/4][j/4] + rand()%3 - 1;
        //             temp[i][j] = temp4[i/4][j/4];
        //         }
        //     }

        // }
        
        std::map<std::string, std::string> tileID = {
            {"Air", ""},
            {"Stone", "./Image/Stone.png"},
            {"SnowStone", "./Image/SnowStone.png"},
            {"CoolStone", "./Image/CoolStone.png"},
            {"WarmStone", "./Image/WarmStone.png"},
            {"HotStone", "./Image/HotStone.png"},
            {"Grass", "./Image/Grass.png"},
            {"ColdGrass", "./Image/ColdGrass.png"},
            {"CoolGrass", "./Image/CoolGrass.png"},
            {"FloweryGrass", "./Image/FloweryGrass.png"},
            {"Dirt", "./Image/Dirt.png"},
            {"Sand", "./Image/Sand.png"},
            {"BlueSand", "./Image/BlueSand.png"},
            {"GreenSand", "./Image/GreenSand,png"},
            {"RedSand", "./Image/RedSand.png"},
            {"StoneBrick", "./Image/StoneBrick.png"},
            {"Water", "./Image/Water.png"},
            {"Lava", "./Image/Lava.png"},
            {"Shore", "./Image/Shore.png"},
        };

        std::map<std::string, std::string> mobID = {
            {"Snake", "./Image/Snake.png"},
            {"Robot", "./Image/Robot.png"},
            {"Cod", "./Image/cod.png"},
        };

        std::map<std::string, std::string> itemID = {
            {"Apple", "./Image/Apple.png"},
            {"Watermelon", "./Image/Watermelon.png"},
            {"Carrot", "./Image/Carrot.png"},
        };
};
#endif