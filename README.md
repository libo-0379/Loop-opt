文件说明：
1. O0、O1、O2、O3 分别是G++优化参数
2. jh：交换，内外层循环索引交换优化，详见代码有注释
3. hb：合并，循环合并优化
4. tc：提出，循环内函数调用或者计算结果保存复用
5. run-all.sh：用9组 yhbatch命令跑对比测试
6. out：是对应测试的时钟周期结果

测试结果：
<img width="1204" alt="image" src="https://github.com/user-attachments/assets/bd598764-d850-4678-bbb1-d5d6ee35456d">
