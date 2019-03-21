package file_daemond;

import java.util.List;
import java.util.Map;

public interface Bizable {
	public static final long versionID = 1L;
	boolean discover(List<String> meta_path, List<Map> log);
	void ResetDispatcher();
	void closeServer();
}
