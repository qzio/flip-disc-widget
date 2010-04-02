class DisplayQueue
  include MongoMapper::Document

  key :str, String
  key :published_at, Time
  timestamps!

  validates_length_of :str, :minimum => 1, :maximum => 40, :message => 'be nice say something'

  before_save :sanitize_str

  def sanitize_str
    self.str = self.str.gsub(/[^A-Za-z0-9åäöÅÄÖ_!?\-\s]\./,'_')
  end
end
