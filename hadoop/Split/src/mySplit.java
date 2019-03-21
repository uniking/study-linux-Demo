import java.util.Iterator;

import org.ansj.domain.Term;
import org.ansj.splitWord.analysis.ToAnalysis;

public class mySplit {

	public static void main(String[] args) throws Exception
	{
		String words = "中国是世界四大文明古国之一，有着悠久的历史，距今约5000年前，以中原地区为中心开始出现聚落组织进而成国家和朝代，后历经多次演变和朝代更迭，持续时间较长的朝代有夏、商、周、汉、晋、唐、宋、元、明、清等。中原王朝历史上不断与北方游牧民族交往、征战，众多民族融合成为中华民族。20世纪初辛亥革命后，中国的君主政体退出历史舞台，取而代之的是共和政体。1949年中华人民共和国成立后，在中国大陆建立了人民代表大会制度的政体。中国有着多彩的民俗文化，传统艺术形式有诗词、戏曲、书法和国画等，春节、元宵、清明、端午、中秋、重阳等是中国重要的传统节日。";  
        //System.out.println(ToAnalysis.parse(words));
        
        Iterator<Term> itor = ToAnalysis.parse(words).iterator();
        
        while(itor.hasNext())
        	System.out.println(itor.next());
	}
}
