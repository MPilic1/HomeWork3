#include <iostream>
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <chrono> 
#include <thread> 

const int ROWS = 20;
const int COLS = 40;

struct Point {
    int row, col;
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};


struct Compare {
    bool operator()(const std::pair<int, Point>& a, const std::pair<int, Point>& b) {
        return a.first > b.first;
    }
};

bool is_valid(int r, int c) {
    return r >= 0 && r < ROWS && c >= 0 && c < COLS;
}

std::vector<Point> dijkstra(const std::vector<std::vector<int>>& grid, const Point& start, const Point& goal) {
    const int INF = std::numeric_limits<int>::max();
    std::vector<std::vector<int>> dist(ROWS, std::vector<int>(COLS, INF));
    std::vector<std::vector<Point>> came_from(ROWS, std::vector<Point>(COLS, { -1, -1 }));
    using PQElement = std::pair<int, Point>;
    std::priority_queue<PQElement, std::vector<PQElement>, Compare> pq;
    std::vector<Point> directions = { {0, 1}, {1, 0}, {0, -1}, {-1, 0} };

    dist[start.row][start.col] = 0;
    pq.push(std::make_pair(0, start));

    while (!pq.empty()) {
        PQElement current_element = pq.top();
        pq.pop();
        int current_dist = current_element.first;
        Point current = current_element.second;

        if (current == goal) {
            std::vector<Point> path;
            Point p = goal;
            while (p != start) {
                path.push_back(p);
                p = came_from[p.row][p.col];
            }
            path.push_back(start);
            std::reverse(path.begin(), path.end());
            return path;
        }

        for (int i = 0; i < 4; ++i) {
            Point neighbor = { current.row + directions[i].row, current.col + directions[i].col };
            if (is_valid(neighbor.row, neighbor.col) && grid[neighbor.row][neighbor.col] != 1) {
                int new_dist = current_dist + 1;
                if (new_dist < dist[neighbor.row][neighbor.col]) {
                    dist[neighbor.row][neighbor.col] = new_dist;
                    came_from[neighbor.row][neighbor.col] = current;
                    pq.push(std::make_pair(new_dist, neighbor));
                }
            }
        }
    }

    return {};
}

void print_grid(const std::vector<std::vector<char>>& grid, const std::vector<Point>& path) {
    std::vector<std::vector<char>> temp_grid = grid;
    for (const Point& p : path) {
        temp_grid[p.row][p.col] = 'o';
    }
    for (const auto& row : temp_grid) {
        for (char cell : row) {
            std::cout << cell;
        }
        std::cout << '\n';
    }
    std::cout << std::endl;
}

void clear_screen() {
    std::cout << "\033[2J\033[1;1H";
}

int main() {
    std::vector<std::vector<char>> grid(ROWS, std::vector<char>(COLS, '.'));
    std::vector<std::vector<int>> cost_grid(ROWS, std::vector<int>(COLS, 0));

    int start_row, start_col, end_row, end_col;
    std::cout << "Enter the start row (1-20): ";
    std::cin >> start_row;
    std::cout << "Enter the start column (1-40): ";
    std::cin >> start_col;
    std::cout << "Enter the end row (1-20): ";
    std::cin >> end_row;
    std::cout << "Enter the end column (1-40): ";
    std::cin >> end_col;

    Point start = { start_row - 1, start_col - 1 };
    Point end = { end_row - 1, end_col - 1 };

    grid[start.row][start.col] = 'A';
    grid[end.row][end.col] = 'B';

    int num_walls;
    std::cout << "Enter the number of walls: ";
    std::cin >> num_walls;

    for (int i = 0; i < num_walls; ++i) {
        int wall_row, wall_col;
        std::cout << "Enter wall " << i + 1 << " row (1-20): ";
        std::cin >> wall_row;
        std::cout << "Enter wall " << i + 1 << " column (1-40): ";
        std::cin >> wall_col;
        grid[wall_row - 1][wall_col - 1] = '#';
        cost_grid[wall_row - 1][wall_col - 1] = 1;
    }

    std::vector<Point> path = dijkstra(cost_grid, start, end);

    if (path.empty()) {
        std::cout << "No path found." << std::endl;
    }
    else {
        for (size_t i = 0; i < path.size(); ++i) {
            print_grid(grid, std::vector<Point>(path.begin(), path.begin() + i + 1));
            std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
            clear_screen();
        }
    }

    return 0;
}
