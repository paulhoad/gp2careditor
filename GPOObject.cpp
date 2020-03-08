#include "stdafx.h"

#include "Car.h"
#include "CarTree.h"

#include "resource.h"

#include "CarEditorDoc.h"
#include "CarEditorView.h"

void GPOObject::LoadTree(CCarTree *tree)
{
	char buffer[256];
	carNode = tree->insertInfoNode(tree->m_CarTree.GetRootItem(),"GPO Object",TO_ID(IDB_HELMET));

	wsprintf(buffer,"Points %d",numPoints);
	HTREEITEM pts = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POINT));
	for(int i=0;i<numPoints;i++)
	{
	   if (!pointData[i].getDesc().empty())
	   {
		 wsprintf(buffer,"p[%d] %d %d %d %s",i,pointData[i].x,pointData[i].y,pointData[i].z,pointData[i].getDesc().c_str());
	   }
	   else
	   {
	     wsprintf(buffer,"p[%d] %d %d %d",i,pointData[i].x,pointData[i].y,pointData[i].z);
	   }
	   HTREEITEM ptItem = tree->insertInfoNode(pts,buffer,TO_ID(IDB_POINT));
	   tree->m_CarTree.SetItemData(ptItem,(DWORD_PTR)&pointData[i]);
	   pointData[i].node = ptItem;
	   pointData[i].idx = i;
	   pointData[i].object = this;
	}

	wsprintf(buffer,"Verticies %d",numVerticies);
	HTREEITEM vertex = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(int i=0;i<numVerticies;i++)
	{
	  
	   sprintf(buffer,"v[%d] %d -> %d =%2.2fcm",i+1,vertexData[i].from,vertexData[i].to,vertexData[i].LengthCM());
	   HTREEITEM verItem = tree->insertInfoNode(vertex,buffer,TO_ID(IDB_VERTICIES));
	   tree->m_CarTree.SetItemData(verItem,(DWORD_PTR)&vertexData[i]);
	   vertexData[i].node = verItem;
	   vertexData[i].idx = i;
	   vertexData[i].object = this;
	}

	wsprintf(buffer,"ScaleData %d",numScale);
	HTREEITEM scalep = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_SCALE));
	for(int i=0;i<numScale;i++)
	{
	  
	   if (i<31)
	   {
	     wsprintf(buffer,"s[%d] %d",i,scaleData[i].val);
	   }
	   else
	   {
		 wsprintf(buffer,"s[%d] %d",i,scaleData[i].val);
	   }
	   HTREEITEM scaleItem = tree->insertInfoNode(scalep,buffer,TO_ID(IDB_SCALE));
	   scaleData[i].node = scaleItem;
	   scaleData[i].idx = i;
	   scaleData[i].object = this;
	   tree->m_CarTree.SetItemData(scaleItem,(DWORD_PTR)&scaleData[i]);
	}


	/*
	wsprintf(buffer,"TextureData %d",textureSize);
	HTREEITEM textureP = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_POLYGONS));

	for(i = 0; i< textureSize;i++)
	{
		char argsStr[1024];

		strcpy(argsStr,"Args=");

		for(int j=0;j<textureData[i].numArgs;j++)
		{
			char msg[20];
			wsprintf(msg,"%x ",textureData[i].Args[j]);
			strcat(argsStr,msg);
		}

		wsprintf(buffer,"[%d] %d %d (0x%x) %s",i,textureData[i].numl,textureData[i].numh,textureData[i].cmd,argsStr);
		HTREEITEM textItem = tree->insertInfoNode(textureP,buffer,TO_ID(IDB_POLYGONS));
		HTREEITEM textureItem = textureData[i].LoadTree(tree,textItem);
		textureData[i].node = textItem;
		textureData[i].idx = i;
		tree->m_CarTree.SetItemData(textItem,(DWORD)&textureData[i]);


	}


	wsprintf(buffer,"UnkData %d",numUnks);
	HTREEITEM unkp = tree->insertInfoNode(carNode,buffer,TO_ID(IDB_VERTICIES));
	for(i=0;i<numUnks;i+=8)
	{
	  
	   wsprintf(buffer,"u[%d] %d %d %d %d %d %d %d %d",i,
				unkData[i].data,
				unkData[i+1].data,
				unkData[i+2].data,
				unkData[i+3].data,
				unkData[i+4].data,
				unkData[i+5].data,
				unkData[i+6].data,
				unkData[i+7].data);
	   HTREEITEM verItem = tree->insertInfoNode(unkp,buffer,TO_ID(IDB_VERTICIES));
	   //tree->m_CarTree.SetItemData(verItem,(DWORD)&vertexData[i]);
	   unkData[i].node = verItem;
	   unkData[i].idx = i;
	}
	*/

}


void GPOObject::parse(int begin)
{
	header = (t_objheader*)&data;
	memoryOffset = begin;

	if (header->Magic!= 0xffff8002)
	{
		char msg[256];
		wsprintf(msg,"Bad Magic Number in exe 0x%x",header->Magic);
		AfxMessageBox(msg,MB_OK);
		valid = FALSE;
		return;
	}

	valid = TRUE;

	int start=106;

	TRACE("Magic %x\n",header->Magic);
	TRACE("Id %x\n",header->id);
	TRACE("scaleBegin %x\n",header->scaleBegin);
	TRACE("scaleEnd %x\n",header->scaleEnd);
	TRACE("textureBegin %x\n",header->textureBegin);
	TRACE("textureEnd %x\n",header->textureEnd);
	TRACE("pointsBegin %x\n",header->pointsBegin);
	TRACE("fileEnd %x\n",header->fileEnd);
	TRACE("vertexEnd %x\n",header->vertexEnd);
	int diff = header->vertexBegin-header->pointsBegin;
	TRACE("pointsBegin %x %d\n#######",diff,diff);

	TRACE("scaleBegin=%x\n",start+(header->scaleBegin-header->scaleBegin));
	TRACE("scaleEnd=%x\n",start+(header->scaleEnd-header->scaleBegin));
	TRACE("textureBegin=%x\n",start+(header->textureBegin-header->scaleBegin));
	TRACE("textureEnd=%x\n",start+(header->textureEnd-header->scaleBegin));
	TRACE("pointsBegin=%x\n",start+(header->pointsBegin-header->scaleBegin));

	numVerticies = header->size8/8;
	numScale  = (header->scaleEnd - header->scaleBegin)/2;
	numPoints =	(header->vertexBegin - header->pointsBegin)/8;
	numUnks   = (header->fileEnd -header->vertexEnd)/2;

	TRACE("numVerticies=%d\n",numVerticies);

	numVerticies = (header->vertexEnd - header->vertexBegin)/4;

	TRACE("numVerticies=%d\n",numVerticies);
	TRACE("numScale=%d\n",numScale);
	TRACE("numPoints=%d\n",numPoints);

	scaleData  = new Scale[numScale];
	pointData  = new Point3D[numPoints];
	vertexData = new Vertex[numVerticies];
	textureData = new TextureCmd[1000];
	unkData = new DatVal[30000];
	updateData(TRUE);
}

void GPOObject::updateData(BOOL updateTexture)
{
	int start=78;

	if (memoryOffset == DEF_CAR_START)
	{
		start = 106;
	}

	for(int i=0;i<numScale;i++)
	{
		int offset = start+(i*2);
		int val = GetPCWord(offset);
		//TRACE("Scale %d = %d\n",i,val);

		if (val > 0x8000) val = -(0xFFFF-val);
		scaleData[i].val    = val;
		scaleData[i].offset = offset;
		scaleData[i].object = this;
	}



	for(int i=0;i<numVerticies;i++)
	{
		int offset = 4+start+(header->vertexBegin-header->scaleBegin)+(i*4);
		//if (memoryOffset == DEF_CAR_START)
	    //{
		//  offset = 0x1d42+(i*4);
		//}
		vertexData[i].offset = offset;
		vertexData[i].object = this;
		int from = GetPCByte(offset);
		int to   = GetPCByte(offset+1);
		int A    = GetPCByte(offset+2);
		int B    = GetPCByte(offset+3);
		//TRACE("Vertex %x %d = %d %d (%d %d)\n",offset,i,from,to,A,B);
		vertexData[i].from = from;
		vertexData[i].to = to;
	}

	for(int i=0;i<numUnks;i++)
	{
	   int offset = start+(header->vertexEnd-header->scaleBegin)+(i*2);
	   int data   = (int)GetPCWord(offset);	
	   unkData[i].data = data;
	}

	for(int i=0;i<numPoints;i++)
	{
		int offset = start+(header->pointsBegin-header->scaleBegin)+(i*8);
		//int offset = 0x1126+(i*8);
		pointData[i].offset = offset;
		pointData[i].object = this;
		int x   = (int)GetPCWord(offset);
		int y   = (int)GetPCWord(offset+2);
		int z   = (int)GetPCWord(offset+4);
		int u   = (int)GetPCWord(offset+6);

		pointData[i].ox = x;
		pointData[i].oy = y;

		//TRACE("%x (0x%x 0x%x 0x%x)\n",offset,x,y,z);

		if (z > 0x8000) z = -(0xFFFF-z);

		if (x < 0x8000)
		{	
		 if (x > 0x80 && x < 0xFF)
		 {
			int idx = (x-0x84)/4;
			x = scaleData[idx].val;
			x = -x;
		 }
		 else if (x > 0)
		 {
		   int idx = (x-0x4)/4;
		   x = scaleData[idx].val;
		 }
		
		 if (y > 0x80 && y < 0xFF)
		 {
			int idx = (y-0x84)/4;
			y = scaleData[idx].val;
			y = -y;
		 }
		 else if (y > 0)
		 {
			int idx = (y-0x04)/4;
			y = scaleData[idx].val;
		
		 }
		}
		else
		{
		   int pidx = x - 0x8000;
		   x = pointData[pidx].x;
		   y = pointData[pidx].y;
		}

		pointData[i].x = x;
		pointData[i].y = y;
		pointData[i].z = z;
	}


	if (updateTexture)
	{
	int count = start+(header->textureBegin-header->scaleBegin);
	int end   = start+(header->textureEnd-header->scaleBegin);

	//textureSize = end-count;

	int idx=0;
	textureSize = 0;

	while(count < end)
	{
		int numl = GetPCByte(count++);
		int numh = GetPCByte(count++);
		int cmd  = GetPCByte(count++);
		textureData[idx].numl = numl;
		textureData[idx].numh = numh;
		textureData[idx].cmd = cmd;
		textureData[idx].object = this;

		int nArgs=0;

		switch(cmd)
		{
		   case 0x80:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  case 0x90:
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].numArgs = nArgs;
			   break;
		  
		  
		  case 0x13:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);

			  if (textureData[idx].Args[3] == 0x80)
			  {
				textureData[idx].Args[nArgs++] = GetPCByte(count++);
			    textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  }
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0x18:
		  case 0x11:
		  case 0x1a:
		  case 0x16:
		  case 0x17:		 
		  case 0x12:
		  case 0x15:
		  //case 0xa:
		  case 0x10:
		  case 0x0:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  
			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		  case 0xa:
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  
			  while(!(textureData[idx].Args[nArgs-2]==0 && textureData[idx].Args[nArgs-1]==0))
			  {
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);
			   textureData[idx].Args[nArgs++] = GetPCByte(count++);			  
			  }
			  //textureData[idx].Args[nArgs++] = GetPCByte(count++);
			  textureData[idx].numArgs = nArgs;
			  break;
		}

		textureData[idx].parse();
		idx++;
		textureSize++;
	  }
    }
}

