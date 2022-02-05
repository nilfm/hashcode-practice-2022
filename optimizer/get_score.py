def compute_score(input_path, output_path):
    with open(output_path, "r") as f:
        ingredients = set(f.read().split()[1:])

    score = 0
    with open(input_path, "r") as f:
        sz = int(f.readline())
        for _ in range(int(sz / 2)):
            likes = f.readline().split()
            n_likes, likes = int(likes[0]), set(likes[1:])
            dislikes = set(f.readline().split()[1:])

            if len(ingredients & likes) == n_likes and len(ingredients & dislikes) == 0:
                score += 1

    return score
