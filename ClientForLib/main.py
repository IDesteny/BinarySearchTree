from matplotlib import pyplot
import pandas
import json
import sys

pyplot.plot(pandas.DataFrame(json.loads(open(sys.argv[1], encoding='utf-16-le').read())))
pyplot.show()