'''
获取ticker数据并保存
注意：安装tushare需要一堆依赖：pip install lxml pandas requests bs4 tushare
'''
import pandas_datareader.data as web
import tushare as ts
import pandas as pd
import datetime
import os
import io
import matplotlib
import matplotlib.pyplot as plt

tickers = ['600848','000002']

'''
通过tushare
'''
# 获取沪深股票列表
# tickersRawData = ts.get_stock_basics()
# # 股票代码
# tickers = tickersRawData.index.tolist()
print(tickers)
# # 上证50
# data = ts.get_sz50s()
# # 沪深300
# data = ts.get_hs300s()
# # 中证500
# data = ts.get_zz500s()
# print(data.head(5))
# # 创业板
# ts.get_hist_data('cyb')
# # 实时行情数据
# data = ts.get_today_all()
# # 历史分笔数据
data = ts.get_tick_data(tickers[0],date='2019-01-01',retry_count=3,pause=2)#重复3次，间隔2秒
print(type(data))
# # 走势图
# matplotlib.style.use('ggplot')
# data.plot(data['price'])

# 保存行情数据
def save_stock_tick(ticker, folder):
	# 历史行情数据，日线
	intraday = ts.get_hist_data(ticker, ktype='D',start='2019-01-01',end='2019-01-31')

	file = folder+'/'+ticker+'.csv'
	if os.path.exists(file):
		history = pd.read_csv(file, index_col=0)#指定第0列作为索引
		intraday.append(history)

	intraday.sort_index(inplace=True)
	intraday.index.name = 'timestamp'

    # 保存索引，gbk编码
	intraday.to_csv(file, index=False, encoding='gbk')
	print ('Intraday for ['+ticker+'] got.')

# 下载前3
dateToday = datetime.datetime.today().strftime('%Y%m%d')
for i, ticker in enumerate(tickers[:3]):
	try:
		print ( i, '/', len(tickers))
		save_stock_tick(ticker, folder='./TickerList_'+dateToday)
	except:
		pass
print('Intraday for all stocks got.')

# 蜡烛图
def stockPricePlot(ticker):
	history = pd.read_csv('./TickerList_'+dateToday+'/'+ticker+'.csv', parse_dates=True, index_col=0)

	close = history['close']
	close = close.reset_index()
	close['timestamp'] = close['timestamp'].map(matplotlib.dates.date2num)

	ohlc = history[['open', 'high', 'low', 'close']].resample('1H').ohlc()
	ohlc = ohlc.reset_index()
	ohlc['timestamp'] = ohlc['timestamp'].map(matplotlib.dates.date2num)

	subplot1 = plt.subplot2grid((2,1), (0,0), rowspan=1, colspan=1)
	subplot1.xaxis_date()
	subplot1.plot(close['timestamp'], close['close'], 'b.')
	plt.title(ticker)

	subplot2,ax = plt.subplot2grid((2,1), (1,0), rowspan=1, colspan=1, sharex=subplot1)
	matplotlib.finance.candlestick_ohlc(ax=subplot2, quotes=ohlc.values, width=0.01, colorup='g', colordown='r')

    # # 设置X轴刻度为日期时间
    # ax.xaxis_date()
    # # X轴刻度文字倾斜45度
    # plt.xticks(rotation=45)
    # plt.title("股票代码：601558两年K线图")
    # plt.xlabel("时间")
    # plt.ylabel("股价（元）")
	plt.show()

# '''
# 通过pandas_datareader
# '''
# data = web.DataReader(tickers[0],'yahoo',start='1/1/2018')
# print(data)

# '''
# 通过请求
# '''
# import requests
# url = 'http://www.nasdaq.com/screening/companies-by-industry.aspx?exchange=NASDAQ&render=download'
# dataString = requests.get(url).content
# tickersRawData = pd.read_csv(io.StringIO(dataString.decode('utf-8')))
# tickers = tickersRawData['Symbol'].tolist()

# from zipline.api import order, record, symbol
# def initialize(context):
#     pass
#
# def handle_data(context, data):
#     order(symbol('AAPL'), 10)
#     record(AAPL=data.current(symbol('AAPL'), 'price'))
