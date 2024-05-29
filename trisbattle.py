import sh
from mako.template import Template
from collections import Counter
from itertools import permutations
from glob import glob
import re
import trisbattle

t = Template(open("main.cpp.mako").read())
gcc = sh.Command("g++")

def render_battle(s1_path, s2_path, out_path="battle.cpp"):
    s1 = open(s1_path).read()
    s2 = open(s2_path).read()
    battle_code = t.render(strategy_body1=s1, strategy_body2=s2)
    open(out_path, "w").write(battle_code)
    gcc(out_path, "-o", "battle")

def run_battle(s1, s2, i):
    battle = sh.Command("./battle")
    print(f"{s1} vs {s2} #{i+1}")
    battle_run = battle()
    print(battle_run)
    return battle_run
def fight_and_count(s1, s2, n):
    render_battle(s1, s2)
    return Counter([run_battle(s1, s2, i).split("\n")[-1] for i in range(n)])

def estrai_nome_file(percorso):
    nome_file = re.search(r'/([^/]+)\.txt$', percorso).group(1)
    return nome_file

def elabora_counter(counter):
    vittorie_player1 = counter.get('Player 1 wins', 0)
    vittorie_player2 = counter.get('Player 2 wins', 0)
    pareggi = counter.get('Draw', 0)
    return (vittorie_player1, vittorie_player2, pareggi)

def tournament(n=10):
    wins_map = {}
    strategies = glob("strategies/*")
    for p in permutations(strategies, 2):
        snames = [estrai_nome_file(s) for s in p] + ["draw"]
        results = elabora_counter(fight_and_count(*p, n))
        print("\nRisultati sfida: ", list(zip(snames, results)))
        for name, count in zip(snames, results):
            if name in wins_map:
                wins_map[name] += count
            else:
                wins_map[name] = count
    print("\n\nRisultati finali: ", wins_map)




