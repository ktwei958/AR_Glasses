#include "main.h"
#include "LCD.h"
#include "spi.h"

// DMA传输完成标志（0：未完成，1：完成）
uint8_t LCD_DMA_TX_Finished = 0;
uint8_t LCD_DMA_Buf_Occupied = 0; // 缓冲区占用标志（0=空闲，1=占用）

void delay_ms(uint16_t xms) {
	HAL_Delay(xms);
}

void LCD_GPIO_Init(void) {
//	LCD_SCLK_Set();
//	LCD_MOSI_Set();
	LCD_RES_Set();
	LCD_DC_Set();
	LCD_CS_Set();
	LCD_BLK_Set();
}

/******************************************************************************
 函数说明：硬件SPI数据传输函数（替换原软件模拟的LCD_Writ_Bus）
 入口数据：dat  要写入的8位数据
 返回值：无
 注意：严格匹配SPI配置（Motorola、MSB、CPOL=Low、CPHA=1 Edge）
 ******************************************************************************/
static void LCD_SPI_Transmit(u8 dat) {
	// 等待SPI外设空闲
	while (HAL_SPI_GetState(&hspi1) != HAL_SPI_STATE_READY)
		;

	// 硬件SPI传输1字节数据（阻塞模式，确保传输完成）
	HAL_SPI_Transmit(&hspi1, &dat, 1, HAL_MAX_DELAY);
}

/******************************************************************************
 函数说明：SPI DMA非阻塞传输（用于大数据：填充颜色）
 入口数据：pData 数据缓冲区地址，Len 数据长度（字节数）
 返回值：无
 注意：传输期间CPU可执行其他任务，通过LCD_DMA_TX_Finished标志判断完成
 ******************************************************************************/
void LCD_SPI_Transmit_DMA(uint8_t *pData, uint16_t Len) {
	while (LCD_DMA_Buf_Occupied == 1)
		; // 缓冲区被占用，拒绝新传输
	LCD_DMA_Buf_Occupied = 1; // 标记占用
	LCD_DMA_TX_Finished = 0;
	HAL_SPI_Transmit_DMA(&hspi1, pData, Len);
}

/******************************************************************************
 函数说明：DMA传输完成回调函数（需在spi.c中调用）
 入口数据：无
 返回值：无
 ******************************************************************************/
void LCD_DMA_TX_Callback(void) {
	LCD_DMA_TX_Finished = 1;
	LCD_DMA_Buf_Occupied = 0; // 传输完成，释放缓冲区
}

/******************************************************************************
 函数说明：LCD串行数据写入函数（复用原逻辑，仅替换SPI传输部分）
 入口数据：dat  要写入的串行数据
 返回值：无
 ******************************************************************************/
void LCD_Writ_Bus(u8 dat) {
	LCD_CS_Clr();  // 拉低CS，选中LCD
	LCD_SPI_Transmit(dat);                                       // 硬件SPI传输
	LCD_CS_Set();    // 拉高CS，结束传输
}

/******************************************************************************
 函数说明：LCD写入8位数据（无修改，复用原逻辑）
 入口数据：dat 写入的数据
 返回值：无
 ******************************************************************************/
void LCD_WR_DATA8(u8 dat) {
	LCD_Writ_Bus(dat);
}

/******************************************************************************
 函数说明：LCD写入16位数据（无修改，复用原逻辑：分两次8位传输）
 入口数据：dat 写入的数据
 返回值：无
 ******************************************************************************/
void LCD_WR_DATA(u16 dat) {
	LCD_Writ_Bus(dat >> 8);  // 先传高8位（MSB）
	LCD_Writ_Bus(dat & 0xFF); // 再传低8位
}

/******************************************************************************
 函数说明：LCD写入16位数据（无修改，复用原逻辑：分两次8位传输）
 入口数据：dat 写入的数据
 返回值：无
 ******************************************************************************/
void LCD_WR_DATA_DMA(u16 dat) {
	uint8_t *dma_buf = (uint8_t*) malloc(2); // 堆分配（需手动释放）
	if (dma_buf == NULL)
		return; // 内存分配失败保护
	dma_buf[0] = (dat >> 8) & 0xFF;    // 高8位（先传）
	dma_buf[1] = dat & 0xFF;
	LCD_SPI_Transmit_DMA(dma_buf, 2);  // 先传高8位（MSB）再传低8位
	while (LCD_DMA_TX_Finished == 0)
		;
	free(dma_buf);
}

/******************************************************************************
 函数说明：LCD写入命令
 入口数据：dat 写入的命令
 返回值：  无
 ******************************************************************************/
void LCD_WR_REG(u8 dat) {
	LCD_DC_Clr(); //写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set(); //写数据
}

/******************************************************************************
 函数说明：设置起始和结束地址
 入口数据：x1,x2 设置列的起始和结束地址
 y1,y2 设置行的起始和结束地址
 返回值：  无
 ******************************************************************************/
void LCD_Address_Set(u16 x1, u16 y1, u16 x2, u16 y2) {
	if (USE_HORIZONTAL == 0) {
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	} else if (USE_HORIZONTAL == 1) {
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1 + 80);
		LCD_WR_DATA(y2 + 80);
		LCD_WR_REG(0x2c); //储存器写
	} else if (USE_HORIZONTAL == 2) {
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	} else {
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1 + 80);
		LCD_WR_DATA(x2 + 80);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	}
}

void LCD_Init(void) {
	LCD_GPIO_Init(); //初始化GPIO

	LCD_RES_Clr(); //复位
	delay_ms(100);
	LCD_RES_Set();
	delay_ms(100);

	LCD_BLK_Set(); //打开背光
	delay_ms(100);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11); //Sleep out 
	delay_ms(120);              //Delay 120ms 
	//************* Start Initial Sequence **********// 
	LCD_WR_REG(0x36);
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);

	LCD_WR_REG(0x3A);
	LCD_WR_DATA8(0x05);

	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x0C);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x33);

	LCD_WR_REG(0xB7);
	LCD_WR_DATA8(0x35);

	LCD_WR_REG(0xBB);
	LCD_WR_DATA8(0x32); //Vcom=1.35V

	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x01);

	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x15); //GVDD=4.8V  颜色深度

	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x20); //VDV, 0x20:0v

	LCD_WR_REG(0xC6);
	LCD_WR_DATA8(0x0F); //0x0F:60Hz        	

	LCD_WR_REG(0xD0);
	LCD_WR_DATA8(0xA4);
	LCD_WR_DATA8(0xA1);

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x31);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0x17);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x15);
	LCD_WR_DATA8(0x31);
	LCD_WR_DATA8(0x34);

	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0xD0);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x0E);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x15);
	LCD_WR_DATA8(0x31);
	LCD_WR_DATA8(0x33);
	LCD_WR_DATA8(0x48);
	LCD_WR_DATA8(0x17);
	LCD_WR_DATA8(0x14);
	LCD_WR_DATA8(0x15);
	LCD_WR_DATA8(0x31);
	LCD_WR_DATA8(0x34);
	LCD_WR_REG(0x21);

	LCD_WR_REG(0x29);
}


/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(u16 x,u16 y,u16 color)
{
	LCD_Address_Set(x,y,x,y);//设置光标位置
	LCD_WR_DATA(color);
}


/******************************************************************************
 函数说明：DMA模式填充颜色（核心修改：大批量数据用DMA传输）
 入口数据：xsta,ysta 起始坐标；xend,yend 终止坐标；color 颜色缓冲区（16位/像素）
 返回值：无
 优化点：避免双重循环，直接将16位颜色数据转为8位字节流，DMA一次性传输
 ******************************************************************************/

void LCD_Fill(u16 xsta, u16 ysta, u16 xend, u16 yend, lv_color_t  *color) {

/*
	 u16 i, j;
	 LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //设置显示范围
	 for (i = ysta; i < yend; i++) {
	 for (j = xsta; j < xend; j++) {
	 LCD_WR_DATA(*color++);
	 }
	 }*/
/*
	uint32_t total_pixels = (xend - xsta) * (yend - ysta);
	// 显式创建8位缓冲区（或使用栈/堆，根据像素数调整）
	uint8_t *dma_buf = (uint8_t*) malloc(2 * total_pixels); // 堆分配（需手动释放）
	if (dma_buf == NULL)
		return; // 内存分配失败保护

	// 拆分16位颜色为：高8位→先传，低8位→后传（符合LCD时序）
	for (uint32_t i = 0; i < total_pixels; i++) {
		dma_buf[2 * i] = (color[i].full >> 8) & 0xFF;    // 高8位（先传）
		dma_buf[2 * i + 1] = color[i].full & 0xFF;       // 低8位（后传）
	}

	LCD_Address_Set(xsta, ysta, xend - 1, yend - 1);
	LCD_CS_Clr();
	LCD_DC_Set();

	// 启动DMA传输（注意：若总字节数>65535，需修改Len为uint32_t）
	LCD_SPI_Transmit_DMA(dma_buf, 2 * total_pixels);
	while (LCD_DMA_TX_Finished == 0); // 阻塞等待

	LCD_CS_Set();
	LCD_DMA_TX_Finished = 0;
	free(dma_buf); // 释放堆内存（避免内存泄漏）*/

//	if(color == NULL) return; // 空指针保护

	    // 1. 计算总像素数和总字节数（16位颜色=2字节/像素）
	    uint32_t total_pixels = (uint32_t)(xend - xsta) * (yend - ysta);
	    uint32_t total_bytes = 2 * total_pixels; // 总传输字节数

	    // 2. 定义单次DMA最大传输字节数（HAL库uint16_t上限）
	    #define BLOCK_MAX_SIZE 65535U // 单次最大65535字节（64KB）
	    uint16_t current_block_size; // 当前块传输长度
	    uint32_t remaining_bytes = total_bytes; // 剩余未传输字节数
	    uint32_t buf_offset = 0; // 缓冲区偏移量（用于分块）

	    // 3. 分配8位DMA传输缓冲区（大小=BLOCK_MAX_SIZE，避免频繁分配）
	    uint8_t *dma_buf = (uint8_t *)malloc(BLOCK_MAX_SIZE);
	    if (dma_buf == NULL) return; // 内存分配失败保护

	    // 4. 配置LCD显示区域和控制引脚
	    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置填充区域
	    LCD_CS_Clr(); // 拉低CS，保持选中（整个分块传输期间不释放）
	    LCD_DC_Set(); // 数据模式（后续传输的是像素数据）

	    // 5. 分块DMA传输循环
	    while (remaining_bytes > 0) {
	        // 计算当前块的传输长度（不超过剩余字节数和BLOCK_MAX_SIZE）
	        current_block_size = (remaining_bytes > BLOCK_MAX_SIZE) ? BLOCK_MAX_SIZE : (uint16_t)remaining_bytes;

	        // 6. 填充当前块的DMA缓冲区（显式拆分16位颜色→8位字节流，解决大小端问题）
	        for (uint16_t i = 0; i < current_block_size; i += 2) {
	            // 计算当前像素在color缓冲区中的索引（buf_offset是字节偏移，需/2转换为像素索引）
	            uint32_t pixel_idx = (buf_offset + i) / 2;
	            u16 pixel_color = color[pixel_idx].full; // 获取16位颜色值

	            // 按LCD要求的顺序：先传高8位，再传低8位（兼容所有CPU）
	            dma_buf[i] = (pixel_color >> 8) & 0xFF;    // 高8位（MSB）
	            dma_buf[i + 1] = pixel_color & 0xFF;       // 低8位（LSB）
	        }

	        // 7. 启动当前块的DMA传输（HAL库uint16_t长度兼容）
	        LCD_SPI_Transmit_DMA(dma_buf, current_block_size);

	        // 8. 等待当前块传输完成（阻塞模式，确保分块顺序正确）
	        while (LCD_DMA_TX_Finished == 0);
	        LCD_DMA_TX_Finished = 0; // 重置完成标志

	        // 9. 更新剩余字节数和缓冲区偏移量
	        remaining_bytes -= current_block_size;
	        buf_offset += current_block_size;
	    }

	    // 10. 传输完成，释放资源
	    LCD_CS_Set(); // 拉高CS，释放LCD
	    free(dma_buf); // 释放DMA缓冲区（避免内存泄漏）
	    dma_buf = NULL; // 避免野指针*/
}


