#include "PlayerExe.h"
#include "Coordinates.h"
#include "AlgorithmMatrix.h"
#include <vector>
#include <algorithm>

enum HeuristicValues // Heuristic values of each posible state.
{
    tie = 0,
    win = 10,
    defeat = -10,
};

namespace ttt
{
    static const int DEPTH_VALUE = 5; // The value of depth that will be use in our algorithm.

    /*The semantics of the values ​​to obtain:
        If the game isn't over, it will return the depth value. Bigger the value, bigger the odds to find a victory path.
        We add to the win and defeat value the depth value, to assure that i'ven the worst win will be always better that any partial state.
            If the actual player wins, the function will return the value (win+depth) minus the amount of levels of depth that we generated.
            If the actual player losses, the function will return the value (-defeat-depth) plus the amount of levels of depth that we generated.
        If there is a tie, it will return the value asociated.*/

    static int calculateValue(int const &gameOverStatus, bool const &isActualPlayer, int const &depth)
    {
        int heuristicValue;
        switch (gameOverStatus)
        {
        case GameStatesValues::currentPlayerWins:
            heuristicValue = (HeuristicValues::win + depth) - (DEPTH_VALUE - depth);
            break;
        case GameStatesValues::currentPlayerDefeated:
            heuristicValue = (HeuristicValues::defeat - depth) + (DEPTH_VALUE - depth);
            break;
        case GameStatesValues::playersTie:
            heuristicValue = HeuristicValues::tie;
            break;
        default:
            heuristicValue = depth;
            if (!isActualPlayer)
            {
                heuristicValue = -heuristicValue;
            }
            break;
        }
        return heuristicValue;
    }

    static int alphaBeta(int depth, int alpha, int beta, bool actualPlayer, AlgorithmMatrix matrix)
    {
        std::vector<Coordinates> movs = matrix.getMovs();
        bool pruning = false;
        int gameOverValue = matrix.trivialGameOver();
        if ((depth == 0) || gameOverValue > 0) // If we reach a leaf (game over) or in this instance we reach the maximun depth value.
        {
            return calculateValue(gameOverValue, actualPlayer, depth);
        }
        else
        {
            std::vector<Coordinates> movs = matrix.getMovs(); // We get all the available movements in the board.
            if (actualPlayer)
            {
                int value = -std::numeric_limits<int>::max();
                for (Coordinates candidate : movs)
                {
                    if (!pruning)
                    {
                        matrix.executeMovement(candidate, actualPlayer);                                   // we execute the movement.
                        value = std::max(value, alphaBeta(depth - 1, alpha, beta, !actualPlayer, matrix)); // we search the maximun value for the current player.
                        alpha = std::max(alpha, value);
                        if (alpha >= beta) // if we already found a better movement.
                        {
                            pruning = true; // we dont need to keep analizing more posible states in this branch.
                        }
                        matrix.undoMovement(candidate); // We undo the movement.
                    }
                }
                return value;
            }
            else
            {
                int value = std::numeric_limits<int>::max();
                for (Coordinates candidate : movs)
                {
                    if (!pruning)
                    {
                        matrix.executeMovement(candidate, actualPlayer);
                        value = std::min(value, alphaBeta(depth - 1, alpha, beta, !actualPlayer, matrix)); // we search the lesser defeat for the current player.
                        beta = std::min(beta, value);
                        if (beta <= alpha) // if we already found a better movement.
                        {
                            pruning = true; // we dont need to keep analizing more posible states in this branch.
                        }
                        matrix.undoMovement(candidate); // We undo the movement.
                    }
                }
                return value;
            }
        }
    }

    static Coordinates play(Board const &board, AlgorithmMatrix & matrix) // Function that obtains the final move to make.
    {
        Coordinates coords{0, 0};
        if (board.valid())
        {
            std::vector<Coordinates> movs = matrix.getMovs();
            bool actualPlayer = true;
            int alpha = -std::numeric_limits<int>::max(); //-Infinite
            int beta = std::numeric_limits<int>::max();   // Infinite.
            int bestAlpha = alpha;
            for (Coordinates candidate : movs)
            {
                matrix.executeMovement(candidate, actualPlayer); // We consume a tile.
                alpha = std::max(alpha, alphaBeta(DEPTH_VALUE, alpha, beta, !actualPlayer, matrix));
                if (alpha > bestAlpha)
                {
                    bestAlpha = alpha;
                    coords = candidate;
                }
                matrix.undoMovement(candidate); // We undo the movement.
            }
            movs.clear();
        }
        return coords;
    }

    // PlayerExe.h Implementations.

    PlayerExe::PlayerExe(std::string const &name) : mName(name)
    {
    }

    std::string const &PlayerExe::name() const
    {
        return mName;
    }

    Coordinates PlayerExe::play(Board const &board) const
    {
        AlgorithmMatrix matrix(board, name());
        return ttt::play(board, matrix);
    }

} // namespace ttt