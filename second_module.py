from abc import ABC

class ExporterFactory(ABC):
    """
    Factory that represents a combination of video and audio codecs.
    The factory doesn't maintain any of the instances it creates.
    """

    def get_video_exporter(self) -> VideoExporter:
        """Returns a new video exporter instance."""
        
    def get_audio_exporter(self) -> AudioExporter:
        """Returns a new audio exporter instance."""

class FastExporter(ExporterFactory):
    """Factory aimed at providing a high speed, lower quality export."""

    def get_video_exporter(self) -> VideoExporter:
        return H264BPVideoExporter()
        
    def get_audio_exporter(self) -> AudioExporter:
        return AACAudioExporter()
    
class HighQualityExporter(ExporterFactory):
    """Factory aimed at providing a high speed, lower quality export."""

    def get_video_exporter(self) -> VideoExporter:
        return H264BPVideoExporter()
        
    def get_audio_exporter(self) -> AudioExporter:
        return AACAudioExporter()

def read_exporter() -> ExporterFactory:
    """Constructs an exporter factory based on the user's preference."""
    factories = {
            "low": FastExporter(),
            "high": HighQualityExporter(),
            "master": MasterQualityExporter()
    }
def main() -> None:
    courses = ['History', 'Math', 'Physics', 'CompSci']
    print(courses)

if __name__ == "__main__":
    main()
