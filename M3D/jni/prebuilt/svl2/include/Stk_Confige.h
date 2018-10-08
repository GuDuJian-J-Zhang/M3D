/**********************************************************
*		Created on 2013/9/3 By Gaoqq
*	All Rights Reserved. Copyright(C) 2012 HOTEAMSOFT
*
*	Function:
*		输出的版本控制
**********************************************************/

/*
 *  当前输出的文件版本
 */
#define SVL_DATAMANAGER_VERSION		L"2.356"		/*beta12(2016.08.16)*/
// 关于版本号的一般原则：
// 架构的改动升级小数点后1位
// 新功能的开发小数点后2位
// 修复BUG、小改善（有结构体调整）升级小数点后3位
// 修复BUG、小改善（无结构提调整）升级beta号

// 版本"2.11"升级内容： <br>
// ① PMI增加3DDim、文本是否平行屏幕显示、显隐状态 <br>
// ② PMI的Leader可以支持末端符号线条 <br>
// ③ VIEW的定义类型 <br>
// ④ 增加预览图 <br>
// ⑤ 增加配置项 <br>

// 版本"2.12"升级内容： <br>
// ① 由SV6.0输出 <br>
// ② 支持读取SV6.0输出的SVL文件 <br>

// 版本"2.13"升级内容： <br>
// ① 增加ProtoTypeKey（PMI以XML方式存储的临时方案用） <br>
// ② 增加辅助线 <br>

// 版本"2.14"升级内容
// ① 增加“解析面”基本属性：平面、圆柱面、圆柱面、球面 <br>
// ② 增加“3D批注”的管理 <br>
// ③ 增加“保存”功能 <br>
// ④ 视图增加“透明度” <br>
// ⑤ 接口的规范化改善 <br>

// 版本"2.145"升级内容
// ① 增加线缆 <br>
// ② 支持转换器使用 <br>

// 版本"2.15"升级内容
// ① 增加"多文件管理策略类":支持单一/分散文件的加载与保存 <br>
// ② 增加“插入子配置”、“修改子配置”、“添加子配置” <br>
// ③ 增加自定义属性 <br>
// ④ 增加线集（兼容线缆） <br>
// ⑤ 增加Instance->Proto、Edge->Face->Mesh的逆向检索

// 版本"2.151"升级内容
// ① 增加BodyName、PMIName

// 版本"2.152"升级内容
// ① 将asm、prt类型直接保存到文件中（原来打开时根据是否有子实例来判断）
// ② 修改ID管理器，支持创建多个DocManager
// ③ 改善分散存储: 改善ProtoType名作为文件名的方式

// 版本"2.153"升级内容
// ① PMI的延长线增加显隐状态

// 版本"2.154"升级内容
// ① 添加PMI的来源

// 版本"2.155"升级内容
// ① 对应cgr转换缺陷，增加ProtoType名的长度为256位，增加IsCatalog标志

// 版本"2.156"升级内容
// ① 为Stk_Text增加应用类型
// ② 增加Andriod系统支持。注意！受系统制约，Andriod输出单个文件不能超过2G

// 版本"2.157"升级内容
// ① 为Stk_Text增加包围盒

// 版本"2.158"升级内容
// ① 为对应ProE转换线缆零件，增加IsPipe标志
// ② 增加线集类型的枚举，如（线缆枚举）
// ③ 为Stk_CurveEntity类的显隐状态设置默认值

// 版本"2.16"升级内容
// ① 为ProtoType增加长度单位
// ② 为Stk_Text增加外框

// 版本"2.161"升级内容
// ① 增加用户数据段

// 版本"2.162"升级内容
// ① 增加Stk_ProtoType::GetViewByID、Stk_OutFrame::CurveByIndex接口
// ② 修正打开SVL状态返回
// ③ 支持打开SV7.0输出的装配组件
// ④ 为RegisterID增加错误状态
// ⑤ 设置Stk_Camera旋转矩阵时，自动转换为UV向量（对于Stk_Camera而言设置矩阵/UV向量对SVL是等价的）
// ⑥ 增加删除UserData接口

// 版本“2.20”升级内容
// ① 支持UG、CATIA转换
// ② 增加Node层
// ③ ProtoType名称扩大至256字符
// ④ 增加空间点

// 版本“2.21”升级内容
// ① 稳定2.20版的功能

// 版本“2.22”升级内容
// ① 增加视图与图层的关联
// ② 修复Andriod版本写文件时的字符串处理
// ③ 为PMI增加AddMetaData相关接口
// ④ 为Body、MeshCom、PMI、View、Note、LayerInfoset、MetaData增加GetNode接口

// 版本“2.311”升级内容
// ① 增加PMI描述数据

// 版本“2.33”升级内容
// ① SVL2.2新功能稳定版（不含PMI描述数据）
// ② 修改V2.2之前的SVL文件中有关Node ID与PMI ID相同时的逻辑处理；
// ③ 删除PMI与视图关联时，输入参数由PMI ID修改为其所对应的Node ID；
// ④ 修复无权限写入C盘时挂机的问题
// ⑤ 修复删除文档释放内存慢的问题
// ⑥ 修复PMI添加MetaData接口的缺陷；
// ⑦ 修复prt插入子件的问题
// ⑧ 修复日志文件初始化问题
// ⑨ 修复只转当前实例场景下的File释放问题
// ⑩ PMI增加UserData，删除MetaData.

// 版本“2.341”升级内容
// ① 增加实例、特征编号；
// ② 增加焊点Node类型
// ③ 增加PMI是否调整标志
// ④ 增加特征名称
// ⑤ 修正读取组数量、实例编号错误的问题
// ⑥ 为MeshCom增加UserData
// ⑦ 应SView要求，增加焊接符号标注类型和零组件标注类型

// 版本“2.342”升级内容
// ① 增加Body删除Mesh功能；增加MeshEdge宽度；增加Mesh显隐属性；

// 版本“2.344”升级内容
// ① 增加默认视图、实例状态；磁力线、包络框；

// DataManager_V2.345 beta1(2015.04.14)
// ① 增加实例唯一标识GUID；
// ② 增加清空Mesh数据的接口；
// ③ 修改锚点接口；

// DataManager_V2.345 beta2(2015.04.17)
// ① 实例增加自定义属性记录；

// DataManager_V2.346 beta1(2015.04.23)
// ① 要素名称可变；
// ② 增加视图与磁力线的关联；
// ③ 增加包络框的边框属性；

// DataManager_V2.346 beta3(2015.04.28)
// ① 修复读取包络框挂机的问题；
// ② 增加支持编辑锚点的逻辑；

// DataManager_V2.346 beta4(2015.05.04)
// ① Stk_Node增加删除NodeConnector的接口；

// DataManager_V2.347 beta1(2015.05.08)
// ① 增加PMI前置标识；
// ② 增加另存指定的单个文件的接口；
// ③ 增加磁力线颜色属性设置；

// DataManager_V2.347 beta2(2015.05.12)
// ① 修改可变长度名称遗留的缺陷；

// DataManager_V2.351 beta1(2015.05.18)
// ① 增加压缩算法；
// ② 增加PMI图片(图章)数据；
// ③ 增加实例删除指定MetaData的接口；

// DataManager_V2.351 beta2(2015.06.08)
// ① 修复合并压缩算法时，Title的版本号没有合并升级的问题；
// ② 增加日志的输出；
// ③ PMI类型中增加图章类型；
// ④ 修复Mesh只能显示，不能隐藏的缺陷；

// DataManager_V2.351 beta3(2015.06.09)
// ① 增加camera中m_bHasMatrix的初始化；
// ② 修改Mesh显隐版本兼容的问题；

// DataManager_V2.351 beta4(2015.06.09)
// ① 解决康延钦提出的Mesh显隐的缺陷；

// DataManager_V2.351 beta5(2015.06.16)
// ① 对zip压缩性能优化；
// ② 完善图章功能；

// DataManager_V2.351 beta6(2015.06.18)
// ① 修复写入实例属性挂机的缺陷；

// DataManager_V2.351 beta7(2015.06.25)
// ① 修复输出View LOG挂机的缺陷；
// ② 修改输出LOG文件名称；
// ③ 修复保存用户数据挂机的缺陷；

// DataManager_V2.352 beta1(2015.07.13)
// ① 修复保存用户数据挂机的缺陷；
// ② 修改andriod下压缩算法编译不通过的问题；
// ③ 增加View剖切面类型；
// ④ 修复LOG某些内容乱码问题；

// DataManager_V2.353 beta1(2015.08.14)
// ① 增加Node中删除Node与实例之间的关联的接口；
// ② 增加剖切深度；
// ③ 增加文件加密；
// ④ 增加箭头存储；
// ⑤ 修复以旧版本(V2.162)的SVL替换新的SVL后，无法打开的问题；

// DataManager_V2.353 beta2(2015.08.14)
// ① 修复合并到SView后，SView某些工程受影响无法编译通过的问题；
// ② 修复打开文件时，死循环的问题；

// DataManager_V2.353 beta3(2015.09.06)
// ① 增加SVL属性控件所需的数据；
// ② 增加剖切线类型；

// DataManager_V2.354 beta1(2015.09.09)
// ① 支持纹理存储读取；
// ② 为攻长义增加Pick点、PMI类型；

// DataManager_V2.354 beta2(2015.09.09)
// ① 解决andriod无法编译通过的问题；

// DataManager_V2.354 beta3(2015.09.24)
// ① 解决SView反馈的保存单个文件，数据丢失的问题；

// DataManager_V2.354 beta4(2015.09.30)
// ① 增加ProtoType获取File指针是否为空判断，防止挂机；

// DataManager_V2.354 beta5(2015.10.29)
// ① 修复插入V2.2之前版本SVL后，内存无法完全释放的问题；

// DataManager_V2.354 beta6(2015.11.04)
// ① 增加CURVE ID的注册；
// ② 修复通过ID无法获取纹理的问题；
// ③ 修复读取纹理数据错误的问题；

// DataManager_V2.354 beta7(2015.11.16)
// ① 修复不存储MeshEdge的情况下，纹理数据无法写入文件的问题；

// DataManager_V2.354 beta8(2015.11.19)
// ① 修复CURVE ID注册时，无法获取ProtoType的问题；
// ② 修复LINUX下无法编译通过的问题；

// DataManager_V2.354 beta9(2015.12.01)
// ① 修复一个File中存在多个ProtoType，且ProtoType具有装配关系时，内存释放挂机的问题；
// ② 修复SView另存单个文件时，类似CGR、BIM文件挂机的问题；
// ③ 更改引用文件大小写匹配问题；

// DataManager_V2.354 beta10(2015.12.07)
// ① 修复内存泄露问题；
// ② 修复保存特征挂机的问题；

// DataManager_V2.354 beta12(2015.12.23)
// ① 修复内存释放挂机的缺陷；

// DataManager_V2.354 beta13(2015.12.29)
// ① 增加获取文件保存路径的接口；

// DataManager_V2.354 beta14(2016.01.11)
// ① 增加拷贝装配/零件的接口；
// ② 针对转换器不再去重边界线，DM做对应修改；

// DataManager_V2.355 beta1(2016.01.29)
// ① 修改纹理存储结构，存储于File中，并兼容V2.355之前格式的纹理；

// DataManager_V2.355 beta2(2016.01.30)
// ① 修复纹理存储死循环的问题；
// ② 增加标记数据的存储；

// DataManager_V2.356 beta1(2016.03.09)
// ① 增加工艺BOM表格数据的存储；
// ② 修改代码走查中的内存没有释放问题；
// ③ “中广核”合并SVL后压缩时挂机修复，使用临时方案。

// DataManager_V2.356 beta2(2016.03.09)
// ① 修复“中广核”合并SVL后压缩挂机的缺陷；

// DataManager_V2.356 beta3(2016.03.12)
// ① 将“中广核”合并SVL后压缩挂机的临时方案替代为正式方案；

// DataManager_V2.356 beta4(2016.05.06)
// ① 解决转换器因增加“中广核”合并SVL的逻辑后，导致的以前版本转换丢失数据的问题；
// ② 解决Andriod编译BOM表格出错的问题；
// ③ 解决RVM转换Mesh问题。
// ④ 修复内存泄露问题；
// ⑤ 修复混合存储后，修改SVL文件名，重新打开数据丢失的问题；
// ⑥ 增加视图类中删除所有实例属性的接口；

// DataManager_V2.356 beta5(2016.05.27)
// ① 修改装配特征中添加颜色、显隐、材质、配置矩阵的内部逻辑；
// ② 修改没有读取线缆管道标识的缺陷；
// ③ 修复写入图层过滤器的缺陷；

// DataManager_V2.356 beta6(2016.06.02)
// ① 修复保存两次SVL文件后，无法打开的问题，原因在于中广核名字修改导致。

// DataManager_V2.356 beta7(2016.06.06)
// ① 增加删除指定Face的接口；
// ② 修复保存自定义视图属性信息死循环的缺陷；
// ③ 增加视图要素高亮功能；

// DataManager_V2.356 beta8(2016.06.24)
// ① 修复保存存在零件族的数据时，保存两次出现Proto名称>256越界的问题，在保存修改名称检测时，清空map解决；
// ② 增加线缆管道特征类型；

// DataManager_V2.356 beta9(2016.06.27)
// ① 修改注释，去除日文注释。

// DataManager_V2.356 beta10(2016.07.08)
// ① 转换器直转当前实例模式下的File为空的临时处理方案。
// ② 加载SVL文件存在等待一段时间的问题的处理。

// DataManager_V2.356 beta11(2016.08.15)
// ① 增加加载进度监听。

// DataManager_V2.356 beta12(2016.08.16)
// ① 增加面向对象行使的加载进度监听。
// ② 增加获取当前ProtoType的总实例数量和Node数量的接口。
// ③ 解决释放挂机的问题。

#define SVL_FILE_TITLE				L"SVLFILE"

/*
 *  当前输出的记录版本
 */
#define _VER_TITLE_RECORD			6
// ↑版本2 去除数据源的部分信息
// ↑版本3 增加CAD物理文件路径信息
// ↑版本3 扩大CADFileName和DataSource容量
// ↑版本5 增加压缩算法
// ↑版本6 增加文件加密
#define _VER_THUMBNAIL_RECORD		1
#define _VER_HEAD_RECORD			1
#define _VER_OPTION_RECORD			1

#define _VER_PROTO_RECORD			12
// ↑版本2 预留usetype dmy
// ↑版本3 增加分散存储中ChildPath
// ↑版本4 存储Proto的类型
// ↑版本5 提高ProtoType名的字节数、支持Node存储
// ↑版本6 增加长度单位
// ↑版本7 增加CAD物理文件路径
// ↑版本8 增加实例特征编号
// ↑版本9 增加ProtoType的显隐和颜色
// ↑版本10 增加实例标识GUID
// ↑版本11 增加子实例对应的文件的密码
// ↑版本12 增加工艺BOM表格的ID

#define _VER_PROTO_TITLE_RECORD		1
#define _VER_PROTO_KEY_RECORD		1
#define _VER_INS_ATR_RECORD			1
#define _VER_INS_COL_RECORD			1
#define _VER_INS_DSP_RECORD			1
#define _VER_INS_MAT_RECORD			1

#define _VER_BODY_RECORD			5
// ↑版本2 预留usetype dmy
// ↑版本3 增加Body名称
// ↑版本4 扩大Name容量
// ↑版本5 将Body Name作为单独的可变记录

#define _VER_MESH_RECORD			5
// ↑版本2 支持LOD
// ↑版本3 增加MeshType
// ↑版本4 修改Mesh显隐
// ↑版本5 增加Render(渲染)ID
#define _VER_MESH_LOD_RECORD		1
#define _VER_FACE_RECORD			4
// ↑版本2 支持LOD
// ↑版本3 支持MeshEdge
// ↑版本4 支持纹理关联
#define _VER_FACE_LOD_RECORD		1
#define _VER_EDGE_RECORD			1
#define _VER_EDGE_LOD_RECORD		1

#define _VER_PMI_RECORD				8
// ↑版本2 增加显隐状态
// ↑版本3 预留usetype dmy
// ↑版本4 增加PMI名称
// ↑版本5 增加PMI来源
// ↑版本6 增加PMI的方向向量、基准点
// ↑版本7 扩大Name容量
// ↑版本8 将Name作为单独可变记录
#define _VER_LEADER_RECORD			2
// ↑版本2 增加二维折弯线标识
#define _VER_EXLINE_RECORD			2
// ↑版本2 增加显隐状态
#define _VER_LINEAR_RECORD			3
// ↑版本2 增加定义类型
// ↑版本3 增加辅助线的显隐和草绘线的显示类型
#define _VER_LINE_RECORD			3
// ↑版本2 增加定义类型
// ↑版本3 增加辅助线的显隐和草绘线的显示类型
#define _VER_ELLIPSE_RECORD			4
// ↑版本2 增加圆弧的参数
// ↑版本3 增加定义类型
// ↑版本4 增加辅助线的显隐和草绘线的显示类型
#define _VER_COMTEXT_RECORD			4
// ↑版本2 增加复合文本框类型
// ↑版本3 增加锚点、包络框
// ↑版本4 包络框单独作为记录存储，不再放在复合文本中
#define _VER_TEXT_RECORD			5
// ↑版本2 增加应用类型
// ↑版本3 增加Text包围盒
// ↑版本4 增加Text旋转角度
// ↑版本5 增加Text属性、字体风格
#define _VER_OUTFRAME_RECORD		2
// ↑版本2 增加应用类型
#define _VER_ANCHIOR_RECORD			1
#define _VER_SPECIALLINE_RECORD		1

#define _VER_VIEW_RECORD			6
// ↑版本2 预留usetype dmy
// ↑版本3 增加定义类型
// ↑版本4 增加透明度
// ↑版本5 扩大Name容量
// ↑版本6 将Name作为单独可变记录

#define _VER_CAMERA_RECORD			1
#define _VER_CLIP_PLAN_RECORD		2
// ↑版本2 增加深度

#define _VER_NODELINKBASE_RECORD	2
// ↑版本2 预留usetype dmy
#define _VER_PROTOLINK_RECORD		1
#define _VER_NODEID_RECORD			1
#define _VER_SUBENTITY_RECOR		2
// ↑版本2 增加PICK点，为攻长义应用层使用

#define _VER_FACEATTR_RECORD		1
#define _VER_EDGEATTR_RECORD		1
#define _VER_MESHEDGE_RECORD		2
// ↑版本2 增加边界线宽度

#define _VER_NOTE_RECORD			1
// ↑版本1

#define _VER_SUBLINE_RECORD			1
#define _VER_LINESET_RECORD			4
// ↑版本2 增加线集类型
// ↑版本3 扩大Name容量
// ↑版本4 将Name作为单独可变记录

#define _VER_NURBSCURVE_RECORD		1
#define _VER_HYPERBOLA_RECORD		1
#define _VER_PARABOLA_RECORD		1

#define  _VER_METADATA_RECORD		1

#define _VER_Node_RECORD			5
// ↑版本3 扩大Name容量到512
// ↑版本4 将Name单独作为可变记录
// ↑版本5 增加Node Sub类型

//图层
#define _VER_LAYERINFOSSET_RECORD	1
#define _VER_LAYERINFO_RECORD		1
#define _VER_LAYERFILTER_RECORD		1

// 基本信息集Stk_Info
#define _VER_INFO_RECORD			1

// 线缆特征中的Fiber点
#define _VER_PIPEFIBER_PNT_RECORD	1
// 线缆特征
#define _VER_FEAT_PIPE_RECORD		2
// ↑版本2 增加线缆直径/厚度、最小折弯半径等数据
// 特征
#define _VER_FEAT_RECORD			2
// ↑版本2 将特征名称存储为可变长度

#define  _VER_PLCPATHDISPLAY_RECORD	1
// ↑版本1 配置路径与实例显隐映射

// 空间点
#define  _VER_SPACIALPOINT_RECORD	1

// 用户自定义数据
#define _VER_USERDEFDATA_RECORD		2
// ↑版本2 增加名称占位空间
// 参考块
#define _VER_REFERENCEBLOCK_RECORD	1

// 样本公差
#define _VER_TOLSPECIFICATION_RECORD	1

// 样本公差容器
#define _VER_TOLSPECIFICATIONCONTAINER_RECORD	1

// 几何公差
#define _VER_GEOMETRICAL_RECORD			1

// 几何公差的复合文本所属类型
#define _VER_COMTEXTTYPE_RECORD			1
// 尺寸
#define _VER_DIMENSION_RECORD			1
// 基准目标代号
#define _VER_DATUMTARGET_RECORD			1
// 表面粗糙度
#define _VER_ROUGHNESS_RECORD			1
// 焊接符号
#define _VER_WELDING_RECORD				1
// 焊接符号文本所属类型
#define _VER_WELDING_TEXT_RECORD		1
// 基准符号
#define _VER_DATUM_RECORD				1

// 组件特征
#define _VER_COMPONENTFEATURE_RECORD	2
// ↑版本2 增加特征名
#define _VER_VIRTUALFEATURE_RECORD		1
#define _VER_ASSEMBLYFEATURE_RECORD		1

// 孔特征
#define _VER_FEATAXIS_RECORD			1
#define _VER_FEATINNERTHREAD_RECORD		1
#define _VER_FEATHOLE_RECORD			1
// 磁力线特征
#define _VER_FEAT_MAGNETICLINE_RECORD	2
// ↑版本2 增加磁力线颜色
// 默认视图中实例相对于父级的配置矩阵
#define _VER_PLACEMENT_RECORD			1
// 要素名称
#define _VER_ENTITY_NAME_RECORD			1
// 包络框
#define _VER_ENVELOPE_RECORD			1
// 压缩、解压缩
#define _VER_COMPRESS_RECORD			1
// 图片(图章)
#define _VER_PICTURE_RECORD				3
// ↑版本2 增加图片的实际宽高和位图
// ↑版本3 增加缩放因子

// 箭头
#define _VER_FEAT_ARROW_RECORD				2
// ↑版本2 增加定义平面
#define _VER_FEAT_LINE_ARROW_RECORD			1
#define _VER_FEAT_CIRCULAR_ARROW_RECORD		1
#define _VER_FEAT_RIGHTANGLE_ARROW_RECORD	1
// 纹理
#define _VER_TEXTURE_RECORD					1
// 渲染
#define _VER_RENDER_RECORD					1
// UV
#define _VER_UV_RECORD						1
// 剖切线
#define _VER_CLIPPING_CURVE_RECORD			1
// 标记特征
#define _VER_FEAT_MARK_RECORD				1
// 矩形特征标记
#define _VER_FEAT_MARK_RECTANGLE_RECORD		1
// 椭圆特征标记
#define _VER_FEAT_MARK_ELLIPSE_RECORD		1
// 徒手画特征标记
#define _VER_FEAT_MARK_FREEHAND_RECORD		1
// 工艺BOM表格的每一栏
#define _VER_VIRTUALBOM_ITEM_RECORD			1