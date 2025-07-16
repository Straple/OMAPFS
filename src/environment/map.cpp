#include <environment/map.hpp>

#include <utils/assert.hpp>
#include <utils/dsu.hpp>

#include <map>

uint32_t Map::get_rows() const {
    return rows;
}

uint32_t Map::get_cols() const {
    return cols;
}

uint32_t Map::get_size() const {
    return rows * cols + 1;
}

bool Map::is_free(uint32_t pos) const {
    ASSERT(pos < map.size(), "invalid pos");
    return map[pos];
}

std::istream &operator>>(std::istream &input, Map &map) {
    ASSERT(input, "unable to read");
    // TODO: нормально читать
    std::string line;
    std::getline(input, line);
    ASSERT(line == "type octile", "unexpected map file format");
    input >> line;
    ASSERT(line == "height", "unexpected map file format");
    input >> map.rows;
    input >> line;
    ASSERT(line == "width", "unexpected map file format");
    input >> map.cols;
    input >> line;
    ASSERT(line == "map", "unexpected map file format");
    map.map.assign(map.rows * map.cols + 1, false);
    for (uint32_t row = 0; row < map.rows; row++) {
        input >> line;
        ASSERT(line.size() == map.cols, "unexpected map file format");
        for (uint32_t col = 0; col < map.cols; col++) {
            ASSERT(line[col] == '.' || line[col] == '@' || line[col] == 'T', "unexpected cell type");
            map.map[row * map.cols + col + 1] = (line[col] == '.');
        }
    }

    /*get_dsu() = DSU(map.get_size());
    for (uint32_t row = 0; row < map.rows; row++) {
        for (uint32_t col = 0; col < map.cols; col++) {
            if (!map.is_free(row * map.cols + col + 1)) {
                continue;
            }

            auto kek = [&](uint32_t drow, uint32_t dcol) {
                uint32_t row2 = row + drow;
                uint32_t col2 = col + dcol;

                if (row2 < map.rows &&
                    col2 < map.cols && map.is_free(row2 * map.cols + col2 + 1)) {

                    get_dsu().uni(row * map.cols + col + 1, row2 * map.cols + col2 + 1);
                }
            };

            kek(0, +1);
            kek(0, -1);
            kek(+1, 0);
            kek(-1, 0);
        }
    }

    std::map<uint32_t, uint32_t> clusters;
    for (uint32_t row = 0; row < map.rows; row++) {
        for (uint32_t col = 0; col < map.cols; col++) {
            uint32_t pos = row * map.cols + col + 1;

            if (map.is_free(pos)) {
                clusters[get_dsu().get(pos)] = get_dsu().get_size(pos);
            }
        }
    }
    uint32_t best_parent = -1;
    for (auto [parent, size]: clusters) {
        std::cout << parent << ": " << size << '\n';
        if (best_parent == -1 || clusters.at(best_parent) < size) {
            best_parent = parent;
        }
    }
    std::cout << "best: ";
    std::cout << best_parent << ' ' << clusters.at(best_parent) << '\n';

    for (uint32_t row = 0; row < map.rows; row++) {
        for (uint32_t col = 0; col < map.cols; col++) {
            uint32_t pos = row * map.cols + col + 1;

            if (map.is_free(pos)) {
                if(get_dsu().get(pos) != best_parent){
                    map.map[pos] = false;
                }
            }
        }
    }*/

    return input;
}

Map &get_map() {
    static Map map;
    return map;
}
