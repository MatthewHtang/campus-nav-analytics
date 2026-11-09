#!/usr/bin/env python3
"""Build and check M1 in temporary folders; --record writes test_results.txt."""
from pathlib import Path
import subprocess
import tempfile
import textwrap
import sys

ROOT = Path(__file__).resolve().parents[1]
RESULTS = []


def main():
    originals = {p: p.read_bytes() for p in (ROOT / 'data').glob('*.txt')}
    with tempfile.TemporaryDirectory(prefix='campus-m1-') as tmp:
        work = Path(tmp)
        binary = work / 'campus'
        command = ['g++', '-std=c++17', '-Wall', '-Wextra', '-pedantic', '-Iinclude',
                   *[str(p) for p in sorted((ROOT / 'src').glob('*.cpp'))], '-o', str(binary)]
        build = subprocess.run(command, cwd=ROOT, capture_output=True, text=True, check=True)
        print('Build succeeded.' + (' Compiler messages: ' + build.stderr if build.stderr else ' No warnings.'))

        def fixture(name, departments='', facilities='', buildings='Library\n', connections=''):
            folder = work / name
            folder.mkdir()
            for file, content in [('departments', departments), ('facilities', facilities),
                                  ('buildings', buildings), ('connections', connections)]:
                (folder / (file + '.txt')).write_bytes(content.encode())
            return folder

        def check(label, typed, expected, folder='data', absent=()):
            try:
                run = subprocess.run([str(binary), str(folder)], cwd=ROOT, input=typed,
                                     text=True, capture_output=True, timeout=3, check=True)
                missing = [s for s in expected if s not in run.stdout]
                unexpected = [s for s in absent if s in run.stdout]
                passed = not missing and not unexpected
                actual = '; '.join(expected) if passed else 'Missing: ' + repr(missing) + '; unexpected: ' + repr(unexpected)
            except (subprocess.TimeoutExpired, subprocess.CalledProcessError) as error:
                passed, actual = False, str(error)
            folder_label = str(folder)
            if isinstance(folder, Path) and folder.is_relative_to(work):
                folder_label = 'generated/' + folder.name
            RESULTS.append((label, folder_label + '; input ' + repr(typed),
                            '; '.join(expected) + (('; must omit ' + ', '.join(absent)) if absent else ''),
                            actual or 'Exited normally', 'Passed' if passed else 'FAILED'))
            print(RESULTS[-1][-1] + ': ' + label)

        check('Load all sample files', '0\n', ['buildings.txt: 7 accepted, 0 rejected',
              'connections.txt: 6 accepted, 0 rejected', 'facilities.txt: 10 accepted, 0 rejected',
              'departments.txt: 7 accepted, 0 rejected', 'hash capacity 11, entries 10, collisions 1'])
        for value, expected in [('F004', 'Found: F004 | Computer Lab A'), (' f004 ', 'Found: F004 | Computer Lab A'),
                                ('004', 'Invalid format:'), ('F999', 'Not found: no facility with ID F999'),
                                ('', 'Error: ID cannot be blank')]:
            check('ID ' + repr(value), '2\n' + value + '\n0\n', [expected])
        check('Facility display', '1\n0\n', ['F001 | Main Reading Room', 'F010 | Research Help Desk', 'Total facilities: 10'])
        check('Collision and duplicate', '1\n2\nf001\n0\n', ['2 accepted, 1 rejected (hash capacity 11, entries 2, collisions 1)',
              "duplicated ID: 'F001'", 'Found: F001 | Main Reading Room'], 'testdata/collision', ['Duplicate Room |'])
        no_duplicate = fixture('collision_without_duplicate', 'University|NONE\n',
                               'F001|First|Library|Library\nF013|Second|Library|Library\n')
        check('Collision without duplicate', '0\n', ['2 accepted, 0 rejected (hash capacity 11, entries 2, collisions 1)'], no_duplicate)
        category = fixture('category', 'University|NONE\n',
                           'bad|Rejected|Library|COMPUTER LAB\nf001|First|library|Computer Lab\n'
                           'F001|Duplicate|Library|LIBRARY\nF002|Second|Library|computer lab\n'
                           'F003|Third|Library|Library\nF004|Unknown|Missing|Library\nF005|Empty|Library|\n')
        check('Facility validation and spelling', '1\n0\n', ['facilities.txt: 3 accepted, 4 rejected',
              'F001 | First | Library | Computer Lab', 'F002 | Second | Library | Computer Lab',
              'F003 | Third | Library | Library', 'unknown building:', 'field 4 is empty'], category)
        check('Graph order and isolation', '3\n4\n0\n', ['Library: Student Union, Business Building',
              'Observatory: (no connections)', 'Total buildings: 7', 'Total connections: 6',
              'Library - Degree: 2', 'Student Union - Degree: 3', 'Science Building - Degree: 2',
              'Engineering Building - Degree: 1', 'Business Building - Degree: 2', 'Gym - Degree: 2', 'Observatory - Degree: 0'])
        bfs = 'Library - Distance: 0\nStudent Union - Distance: 1\nBusiness Building - Distance: 1\nScience Building - Distance: 2\nGym - Distance: 2\nEngineering Building - Distance: 3\n'
        check('BFS connected order', '5\n library \n0\n', [bfs, 'Reachable: 6 of 7 buildings'])
        check('BFS isolated', '5\nObservatory\n0\n', ['Observatory - Distance: 0', 'Reachable: 1 of 7 buildings'])
        check('BFS unknown', '5\nMissing\n0\n', ['Unknown building "Missing"'])
        check('Invalid graph records', '5\nRoom #5\n0\n', ['buildings.txt: 5 accepted, 2 rejected',
              'connections.txt: 2 accepted, 9 rejected', 'self-connection', 'duplicate connection',
              'unknown building', 'Reachable: 2 of 5 buildings'], 'testdata/member_c_invalid')
        empty = fixture('empty')
        check('Empty structures', '1\n3\n4\n8\n9\n0\n', ['No facilities', 'Library: (no connections)',
              'Total buildings: 1', 'No departments'], empty)
        zero = fixture('zero', buildings='')
        check('Zero-byte files', '1\n3\n4\n8\n0\n', ['Total buildings: 0', 'Total connections: 0',
              'facilities.txt: 0 accepted, 0 rejected', 'departments.txt: 0 accepted, 0 rejected'], zero)
        check('Missing all files', '3\n8\n0\n', ['cannot open file "' + f + '.txt"' for f in
              ['buildings','connections','facilities','departments']], work / 'missing')
        invalid = 'testdata/member_c_invalid'
        check('C load invalid records', '0\n', ['departments.txt: 6 accepted, 9 rejected'], invalid)
        for line in [2, 7, 8, 9, 10, 11, 12, 13, 14]:
            check('C rejected line ' + str(line), '0\n', ['departments.txt, line ' + str(line) + ':'], invalid)
        check('C root without parent prompt', '6\nSchool\n7\nSchool\n8\n9\n0\n',
              ['Department added: School', 'Parent: None\nDirect children: 0\nDepth: 0', 'Preorder: School'], empty, ['Parent department:'])
        check('C root details', '7\nUniversity\n0\n', ['Parent: None\nDirect children: 2\nDepth: 0'])
        check('C child case and spaces', '7\n computer SCIENCE \n0\n',
              ['Department: Computer Science\nParent: College of Engineering\nDirect children: 0\nDepth: 2'])
        check('C missing search', '7\nMissing\n0\n', ['Department "Missing" was not found'])
        check('C add third child', '6\nData Science\nUniversity\n7\nData Science\n7\nUniversity\n0\n',
              ['Department added: Data Science', 'Parent: University', 'Direct children: 3'])
        check('C duplicate add', '6\nuniversity\nUniversity\n7\nUniversity\n0\n',
              ['duplicate department', 'Direct children: 2'])
        check('C unknown parent', '6\nLost\nMissing\n7\nLost\n0\n',
              ['unknown parent "Missing"', 'Department "Lost" was not found'])
        check('C blank names', '6\n\n7\n\n0\n', ['Error: department name cannot be blank.'])
        check('C blank parent', '6\nLost\n\n7\nLost\n0\n', ['parent name is empty', 'Department "Lost" was not found'])
        hierarchy = 'University\n    College of Engineering\n        Computer Science\n        Electrical Engineering\n    College of Science\n        Mathematics\n        Physics\n'
        check('C hierarchy', '8\n0\n', [hierarchy])
        check('C preorder', '9\n0\n', ['Preorder: University -> College of Engineering -> Computer Science -> Electrical Engineering -> College of Science -> Mathematics -> Physics'])
        check('C memory-only restart', '7\nData Science\n0\n', ['Department "Data Science" was not found'])
        check('Menu recovery', '\nabc\n99\n10\n2x\n999999999999999999999\n5\n\n7\nUniversity\n0\n',
              ['Error: enter a menu choice.', 'Error: enter a number from 0 to 9.', 'building name cannot be blank', 'Department: University'])
        for typed in ['0\n', '', '6\n', '6\nChild\n', '7\n', '5\n', '2\n']:
            check('Exit or EOF ' + repr(typed), typed, [])
        windows = fixture('windows', '\ufeff \t# comment\r\n\r\n University | none \r\n Room #5 | UNIVERSITY \r\n',
                          ' f001 | Room #5 | library | Classroom \r\n', ' \tLibrary \r\n')
        check('CRLF and comments', '1\n7\nRoom #5\n0\n', ['departments.txt: 2 accepted, 0 rejected',
              'F001 | Room #5 | Library | Classroom', 'Department: Room #5'], windows)
        assert all(p.read_bytes() == content for p, content in originals.items()), 'Starter data changed!'

    failures = sum(row[-1] != 'Passed' for row in RESULTS)
    print(str(len(RESULTS) - failures) + '/' + str(len(RESULTS)) + ' checks passed; starter data unchanged.')
    if '--record' in sys.argv:
        widths = [4, 28, 42, 46, 46, 10]
        def render(cells):
            wrapped = [textwrap.wrap(str(c), w, break_long_words=True) or [''] for c, w in zip(cells, widths)]
            return '\n'.join(' | '.join((col[i] if i < len(col) else '').ljust(w) for col, w in zip(wrapped, widths)).rstrip()
                             for i in range(max(map(len, wrapped))))
        report = 'TEST RESULTS - Milestone 1\nLocal integration run: 2026-11-08, macOS, g++ (Apple Clang), C++17\n'
        report += 'Executed with Codex assistance. These results are not a CELL certification.\n'
        report += 'Repeat: python3 tests/run_m1_tests.py\nRecord: python3 tests/run_m1_tests.py --record\n'
        report += 'Build: g++ -std=c++17 -Wall -Wextra -pedantic -Iinclude src/*.cpp -o campus\n'
        report += 'Each row runs a fresh process. generated/ inputs are created by the script in temporary folders.\n'
        report += 'Input strings show \\n for Enter. Expected text is checked against actual output.\n'
        report += 'A Passed row means every listed expected fragment appeared, forbidden fragments were absent, and the process exited normally.\n'
        report += 'Build diagnostics: ' + (build.stderr.strip() or 'none') + '\n\n'
        report += render(['#','Test','Input','Expected','Actual','Pass/Fail']) + '\n' + '-+-'.join('-'*w for w in widths) + '\n'
        report += '\n'.join(render([i, *row]) for i,row in enumerate(RESULTS,1))
        report += '\n\nResult: ' + str(len(RESULTS)-failures) + '/' + str(len(RESULTS)) + ' passed. Starter data unchanged.\n'
        report += '\nHistorical evidence: Alka reported six graph/BFS checks passing on UNT CELL18 in the earlier repository test record. That run is not a rerun of this revision.\n'
        report += 'Remaining: compile/run this final combined revision on CELL; verify collaborator access and meaningful commits; match final ZIP to GitHub with no executables.\n'
        (ROOT/'test_results.txt').write_text(report)
    if failures:
        raise SystemExit(1)


if __name__ == '__main__':
    main()
