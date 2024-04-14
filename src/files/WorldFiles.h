#ifndef FILES_WORLDFILES_H_
#define FILES_WORLDFILES_H_

#include "files.h"
#include "../typedefs.h"
#include "../settings.h"
#include "../content/ContentPack.h"
#include "../voxels/Chunk.h"

#include <map>
#include <mutex>
#include <vector>
#include <string>
#include <memory>
#include <unordered_map>
#include <filesystem>
#include <condition_variable>

#include <glm/glm.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/hash.hpp"

inline constexpr uint REGION_HEADER_SIZE = 10;

inline constexpr uint REGION_LAYER_VOXELS = 0;
inline constexpr uint REGION_LAYER_LIGHTS = 1;
inline constexpr uint REGION_LAYER_INVENTORIES = 2;

inline constexpr uint REGION_SIZE_BIT = 5;
inline constexpr uint REGION_SIZE = (1 << (REGION_SIZE_BIT));
inline constexpr uint REGION_CHUNKS_COUNT = ((REGION_SIZE) * (REGION_SIZE));
inline constexpr uint REGION_FORMAT_VERSION = 2;
inline constexpr uint WORLD_FORMAT_VERSION = 1;
inline constexpr uint MAX_OPEN_REGION_FILES = 16;

class Player;
class Content;
class ContentIndices;
class World;

namespace fs = std::filesystem;

class illegal_region_format : public std::runtime_error {
public:
    illegal_region_format(const std::string& message) 
    : std::runtime_error(message) {}
};

class WorldRegion {
    ubyte** chunksData;
    uint32_t* sizes;
    bool unsaved = false;
public:
    WorldRegion();
    ~WorldRegion();

    void put(uint x, uint z, ubyte* data, uint32_t size);
    ubyte* getChunkData(uint x, uint z);
    uint getChunkDataSize(uint x, uint z);

    void setUnsaved(bool unsaved);
    bool isUnsaved() const;

    ubyte** getChunks() const;
    uint32_t* getSizes() const;
};

struct regfile {
    files::rafile file;
    int version;
    bool inUse = false;

    regfile(fs::path filename);
};

using regionsmap = std::unordered_map<glm::ivec2, std::unique_ptr<WorldRegion>>;
using regionproc = std::function<bool(ubyte*)>;

struct RegionsLayer {
    int layer;
    fs::path folder;
    regionsmap regions;
    std::mutex mutex;
};

class WorldFiles {
    std::unordered_map<glm::ivec3, std::unique_ptr<regfile>> openRegFiles;
    std::mutex regFilesMutex;
    std::condition_variable regFilesCv;

    RegionsLayer layers[3] {};

    void writeWorldInfo(const World* world);
    fs::path getRegionFilename(int x, int y) const;
    fs::path getWorldFile() const;
    fs::path getIndicesFile() const;
    fs::path getPacksFile() const;
    
    WorldRegion* getRegion(int x, int z, int layer);
    WorldRegion* getOrCreateRegion(int x, int z, int layer);

    /// @brief Compress buffer with extrle
    /// @param src source buffer
    /// @param srclen length of the source buffer
    /// @param len (out argument) length of result buffer
    /// @return compressed bytes array
    std::unique_ptr<ubyte[]> compress(const ubyte* src, size_t srclen, size_t& len);

    /// @brief Decompress buffer with extrle
    /// @param src compressed buffer
    /// @param srclen length of compressed buffer
    /// @param dstlen max expected length of source buffer
    /// @return decompressed bytes array
    std::unique_ptr<ubyte[]> decompress(const ubyte* src, size_t srclen, size_t dstlen);

    ubyte* readChunkData(int x, int y, uint32_t& length, regfile* file);

    void fetchChunks(WorldRegion* region, int x, int y, regfile* file);

    void writeRegions(int layer);

    ubyte* getData(int x, int z, int layer, uint32_t& size);
    
    std::shared_ptr<regfile> getRegFile(glm::ivec3 coord);
    void closeRegFile(glm::ivec3 coord);
    std::shared_ptr<regfile> useRegFile(glm::ivec3 coord);
    std::shared_ptr<regfile> createRegFile(glm::ivec3 coord);
public:
    static bool parseRegionFilename(const std::string& name, int& x, int& y);
    fs::path getPlayerFile() const;

    fs::path directory;
    std::unique_ptr<ubyte[]> compressionBuffer;
    bool generatorTestMode;
    bool doWriteLights;

    WorldFiles(fs::path directory, const DebugSettings& settings);
    ~WorldFiles();

    void createDirectories();

    void put(Chunk* chunk);
    void put(int x, int z, int layer, std::unique_ptr<ubyte[]> data, size_t size, bool rle);

    std::unique_ptr<ubyte[]> getChunk(int x, int z);
    std::unique_ptr<light_t[]> getLights(int x, int z);
    chunk_inventories_map fetchInventories(int x, int z);

    bool readWorldInfo(World* world);

    void writeRegion(int x, int y, int layer, WorldRegion* entry);

    /// @brief Write all unsaved data to world files
    /// @param world target world
    /// @param content world content
    void write(const World* world, const Content* content);

    void writePacks(const std::vector<ContentPack>& packs);
    void writeIndices(const ContentIndices* indices);

    void removeIndices(const std::vector<std::string>& packs);

    void processRegionVoxels(int x, int z, regionproc func);

    static const inline std::string WORLD_FILE = "world.json";

    fs::path getRegionsFolder(int layer) {
        return layers[layer].folder;
    }
};

#endif /* FILES_WORLDFILES_H_ */
