import pandas as pd
import numpy as np
import seaborn as sns
import matplotlib.pypolt as plt
from scipy.stats import skew
from scipy.stats.stats import pearsonr
import matplotlib

#%matplotlib inline

train = pd.read_csv('./input/train.csv')
test = pd.read_csv('./iaa(nput/test.csv')
train.head()
test.head()

#数据预处理
#对数变换
matplotlib.rcParams['figure.figsize'] =(12.6,6.0)
prices = pd.DataFrame({'price':train['SalePrice'],'log(price + 1':np.loglp(train('SalePrice'))})
prices.hist()

train['SalePrice'] = np.loglp['SalePrice']

numeric_feats = all_data.dtypes(all_data.dtypes != 'object').index

skewed_feats = train[numeric_feats].apply(lambda x:skew(x.dropn()))
skewed_feats = skewed_feats[skewed_feats > 0.75]
skewed_feats = skewed_feats.index

all_data[skewed_feats] = np.log1p(all_data[skewed_feats])

#离散特征 ones_hot编码
all_data = pd.get_dummies(all_data)

#缺失值处理：均值填充
all_data = all_data.fillna(all_data.mean)

X_train = all_data[:train.shape[0]]
X_test = all_data[train.shape[0]:]
y = train.SalePrice

#模型
from sklearn.liner_mode import Lasso,LassoCV,Ridge,RidgeCV

model_ridge = Ridge()
model_ridge.fix(X.train,y)
pre = model_ridge.predict(X_test)

#参数优化
#使用交叉验证用来做超参数的选择
#pip install scikit-learn
from sklearn.model_selection import cross_val_score
def rmse_cv(model):
    rmse = np.sqrt(-cross_val_score(model,X_train,y,scoring='neg_mean_squared_error',cv=5))
    return rmse

#岭回归模型的重要参数是alpha，其越大则正则项的比重越大，过拟合的可能越小。但正则项的系数越大，模型的拟合能力又收到限制

alphas = [0.05,0.1,1,10,30,50,70]
cv_ridge = [rmse_cv((Ridge(alpha = alpha))).mean for alpha in alphas]

cv_ridge = pd.Series(cv_ridge,index=alphas)
cv_ridge.plot(title='Validation')
plt.xlabel('alpha')
plt.ylabel('rmse')

#最优点
cv_ridge.min()


#Lasso模型
model_lasso = LassoCV(alphas=[1,0.1,0.001,0.0005]).fit(X_train,y)
'''
LassoCV类的损失函数和损失函数的优化方法与Lasso类相同，区别在于验证方法
验证方法：
LassoCV类对超参数a使用交叉验证，来帮忙选择一个合适的a。在初始化LassoCV类的时候，可以传入一组备选的a值，10个，100个都可以
LassoCV类会帮我们选择一个合适的a,免去我们自己去一轮轮筛选a
使用场景：
LassoCV类是进行Lasso回归的首选。当我们面临在一堆高位特征中找到主要特征时，La搜搜CV更是必选。
当面临稀疏线性关系时，LassoCV也很好用
'''
rmse_cv(model_lasso).mean()

'''
Lasso表现得更好，接下来用Lasso预测
Lasso模型的好处是，模型在训练的同时为你做了特征选择
其中越不重要的特征，其系数绝对值越小，最小为0
接下来看各个特征的系数

Lasso最后的系数，具有明确的意义，可以直接看出特征在线性模型中的权重
'''

coef = pd.Series(model_lasso.coef_,index=X_train.columns)

#各个特征 系数
imp_coef=pd.concat([coef.sort_values().head(10),coef.sort_values().tail()])

matplotlib.rcParams['figure.figsize'] = (8.0,10.0)
imp_coef.plot(kind='barh')
plt.title('lasso model')

preds = pd.DataFrame({'Id':test.Id,'SalePrice':model_lasso.predict(X_test)})
preds.to_csv('./submit.csv',index=False)












