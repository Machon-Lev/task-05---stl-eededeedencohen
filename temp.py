def get_unique_lines_in_total():
    # read the lines from each file
    with open('north.txt', 'r') as f:
        north_lines = f.read().splitlines()

    with open('south.txt', 'r') as f:
        south_lines = f.read().splitlines()

    # combine north and south lines
    all_lines = north_lines + south_lines

    # find unique lines in total.txt
    with open('total.txt', 'r') as f:
        unique_lines = [line for line in f if line.strip() not in all_lines]

    return unique_lines

print(get_unique_lines_in_total())
